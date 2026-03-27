// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "OasisCameraModeBase.h"

#include "Engine/Canvas.h"
#include "OasisCameraComponentBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OasisCameraModeBase)

//////////////////////////////////////////////////////////////////////////
// UOasisCameraModeBase
//////////////////////////////////////////////////////////////////////////

FName UOasisCameraModeBase::DefaultProxyStateName = "Default";

UOasisCameraModeBase::UOasisCameraModeBase()
{

	BlendTime = 0.5f;
	BlendFunction = EOasisCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	bResetInterpolation = false;
}

UOasisCameraComponentBase* UOasisCameraModeBase::GetOasisCameraComponent() const
{
	return CastChecked<UOasisCameraComponentBase>(GetOuter());
}

UWorld* UOasisCameraModeBase::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

AActor* UOasisCameraModeBase::GetTargetActor() const
{
	if (const UOasisCameraComponentBase* OasisCameraComponent = GetOasisCameraComponent())
	{
		return OasisCameraComponent->GetTargetActor();
	}
	return nullptr;
}

void UOasisCameraModeBase::OnCreated()
{
	InternalOnCreate();
	ReceiveCreation();
}

void UOasisCameraModeBase::OnActivation()
{
	bActive = true;
	for (UOasisCameraProxyBase* Proxy : ProxyInstances)
	{
		if (::IsValid(Proxy))
		{
			Proxy->Activate();
		}
	}
	ReceiveActivation();
}

void UOasisCameraModeBase::OnDeactivation()
{
	for (UOasisCameraProxyBase* Proxy : ProxyInstances)
	{
		if (::IsValid(Proxy))
		{
			Proxy->Deactivate();
		}
	}
	ReceiveDeactivation();
	bActive = false;
}

void UOasisCameraModeBase::ChangeProxyState(const FName& InStateName)
{
	const FName NewStateName = (InStateName == NAME_None)? DefaultProxyStateName : InStateName;
	if (NewStateName == GetCurrentProxyState())
	{
		return;
	}
	CurrentProxyState = NewStateName;
	if (bActive)
	{
		for (UOasisCameraProxyBase* ActiveProxy : ProxyInstances)
		{
			if (ensureAlways(::IsValid(ActiveProxy)))
			{
				ActiveProxy->Deactivate();
			}
		}
	}
	ProxyInstances.Empty();
	TObjectPtr<UOasisCameraProxyClassList>* Config = nullptr;
	if (NewStateName == DefaultProxyStateName)
	{
		Config = &ProxyClasses;
	}
	else
	{
		Config = StateNameToProxyClasses.Find(NewStateName);
	}
	if (Config != nullptr && ::IsValid(*Config))
	{
		for (TSubclassOf<UOasisCameraProxyBase> Class : (*Config)->ClassList)
		{
			TryCreateProxyByClass(Class);
			TObjectPtr<UOasisCameraProxyBase>* Instance = CachedProxyInstances.Find(Class);
			if (Instance != nullptr && ::IsValid(*Instance))
			{
				ProxyInstances.Add(*Instance);
				if (bActive)
				{
					(*Instance)->Activate();
				}
			}
		}
	}
}

FName UOasisCameraModeBase::GetCurrentProxyState() const
{
	return CurrentProxyState;
}

void UOasisCameraModeBase::TryCreateProxyByClass(TSubclassOf<UOasisCameraProxyBase> InClass)
{
	if (CachedProxyInstances.Contains(InClass))
	{
		return;
	}
	UOasisCameraProxyBase* ProxyInstance = NewObject<UOasisCameraProxyBase>(this, InClass, NAME_None, RF_NoFlags);
	if (!ensureAlways(::IsValid(ProxyInstance)))
	{
		return;
	}
	CachedProxyInstances.Emplace(InClass, ProxyInstance);
}

void UOasisCameraModeBase::TryAddDynamicSetting(UOasisCameraSettingBase* InDynamicSetting)
{
	if (!::IsValid(InDynamicSetting))
	{
		return;
	}
	const FName SettingTypeName = InDynamicSetting->GetAssetTypeName();
	if (CachedModeSettings.Contains(SettingTypeName))
	{
		return;
	}
	DynamicModeSettings.Emplace(SettingTypeName, InDynamicSetting);
	CachedModeSettings.Emplace(SettingTypeName, InDynamicSetting);
	TSubclassOf<UOasisCameraSettingRuntimeDataBase> RuntimeDataClass = InDynamicSetting->GetRuntimeDataClass();
	if (RuntimeDataClass)
	{
		SettingRuntimeDataMap.Emplace(SettingTypeName, NewObject<UOasisCameraSettingRuntimeDataBase>(this, RuntimeDataClass));
	}
}

void UOasisCameraModeBase::TryRemoveDynamicSetting(FName SettingTypeName)
{
	if (DynamicModeSettings.Remove(SettingTypeName) <= 0)
	{
		return;
	}
	CachedModeSettings.Remove(SettingTypeName);
	SettingRuntimeDataMap.Remove(SettingTypeName);
}

UOasisCameraSettingRuntimeDataBase* UOasisCameraModeBase::GetSettingRuntimeData(const FName SettingTypeName) const
{
	const TObjectPtr<UOasisCameraSettingRuntimeDataBase>* RuntimeData = SettingRuntimeDataMap.Find(SettingTypeName);
	if (RuntimeData == nullptr)
	{
		return nullptr;
	}
	return (*RuntimeData).Get();
}

FVector UOasisCameraModeBase::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UOasisCameraModeBase::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

const UOasisCameraSettingBase* UOasisCameraModeBase::GetReadOnlySetting(const FName& InSettingType) const
{
	const TWeakObjectPtr<UOasisCameraSettingBase>* SettingPtr = CachedModeSettings.Find(InSettingType);
	if (SettingPtr != nullptr && (*SettingPtr).IsValid())
	{
		return (*SettingPtr).Get();
	}
	return nullptr;
}

void UOasisCameraModeBase::UpdateCameraMode(const FMinimalViewInfo& DefaultCameraView, float DeltaTime)
{
	PreUpdateView(DefaultCameraView, DeltaTime);
	bSuccessUpdated = UpdateView(DefaultCameraView, DeltaTime) || bSuccessUpdated;
	UpdateBlending(DeltaTime);
}

void UOasisCameraModeBase::PreUpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime)
{
	if (!bSuccessUpdated)
	{
		View.AssignedFromMinimalView(DefaultCameraView);
	}
}

bool UOasisCameraModeBase::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime)
{
	TArray<TObjectPtr<UOasisCameraProxyBase>> ProxyInstancesInThisUpdate = ProxyInstances;	// it's ok to change proxy state when update, new state will apply in next update.
	for (UOasisCameraProxyBase* Proxy : ProxyInstancesInThisUpdate)
	{
		if (::IsValid(Proxy))
		{
			Proxy->UpdateView(DefaultCameraView, DeltaTime, View);
		}
	}
	return true;
}

void UOasisCameraModeBase::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	// Since we're setting the blend weight directly, we need to calculate the blend alpha to account for the blend function.
	const float InvExponent = (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f;

	switch (BlendFunction)
	{
	case EOasisCameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;

	case EOasisCameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EOasisCameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EOasisCameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

void UOasisCameraModeBase::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case EOasisCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;

	case EOasisCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EOasisCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EOasisCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

void UOasisCameraModeBase::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("      OasisCameraMode: %s (%f)"), *GetName(), BlendWeight));
}

void UOasisCameraModeBase::InternalOnCreate()
{
	CachedProxyInstances.Empty();
	ProxyInstances.Empty();
	CachedModeSettings.Empty();
	SettingRuntimeDataMap.Empty();
	// for (UOasisCameraSettingBase* Setting : ModeSettings)
	// {
	// 	if (::IsValid(Setting))
	// 	{
	// 		CachedModeSettings.Emplace(Setting->GetPrimaryAssetId().PrimaryAssetType.GetName(), Setting);
	// 	}
	// }
	for (UOasisCameraSettingBase* InstancedSetting : InstancedModeSettings)
	{
		if (::IsValid(InstancedSetting) && !CachedModeSettings.Contains(InstancedSetting->GetPrimaryAssetId().PrimaryAssetType.GetName()))
		{
			CachedModeSettings.Emplace(InstancedSetting->GetPrimaryAssetId().PrimaryAssetType.GetName(), InstancedSetting);
			if (InstancedSetting->GetRuntimeDataClass())
			{
				SettingRuntimeDataMap.Emplace(
				InstancedSetting->GetPrimaryAssetId().PrimaryAssetType.GetName(),
				NewObject<UOasisCameraSettingRuntimeDataBase>(this, InstancedSetting->GetRuntimeDataClass()));
			}
		}
	}
	ChangeProxyState();
}

//////////////////////////////////////////////////////////////////////////
// UOasisCameraModeStack
//////////////////////////////////////////////////////////////////////////
UOasisCameraModeStack::UOasisCameraModeStack()
{
	bIsActive = true;
}

void UOasisCameraModeStack::ActivateStack()
{
	if (!bIsActive)
	{
		bIsActive = true;

		// Notify camera modes that they are being activated.
		for (UOasisCameraModeBase* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnActivation();
		}
	}
}

void UOasisCameraModeStack::DeactivateStack()
{
	if (bIsActive)
	{
		bIsActive = false;

		// Notify camera modes that they are being deactivated.
		for (UOasisCameraModeBase* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnDeactivation();
		}
	}
}

void UOasisCameraModeStack::PushCameraMode(TSubclassOf<UOasisCameraModeBase> CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UOasisCameraModeBase* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();

	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// Already top of stack.
		return;
	}

	// See if it's already in the stack and remove it.
	// Figure out how much it was contributing to the stack.
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->GetBlendWeight());
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	// Decide what initial weight to start with.
	const bool bShouldBlend = ((CameraMode->GetBlendTime() > 0.0f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);

	CameraMode->SetBlendWeight(BlendWeight);

	// Add new entry to top of stack.
	CameraModeStack.Insert(CameraMode, 0);

	// Make sure stack bottom is always weighted 100%.
	CameraModeStack.Last()->SetBlendWeight(1.0f);

	// Let the camera mode know if it's being added to the stack.
	if (ExistingStackIndex == INDEX_NONE)
	{
		CameraMode->OnActivation();
	}
}

bool UOasisCameraModeStack::EvaluateStack(float DeltaTime, FMinimalViewInfo& DefaultCameraView, FOasisCameraModeView& OutCameraModeView)
{
	if (!bIsActive)
	{
		return false;
	}

	UpdateStack(DefaultCameraView, DeltaTime);
	BlendStack(OutCameraModeView);

	return true;
}

TObjectPtr<UOasisCameraModeBase> UOasisCameraModeStack::GetTopMode() const
{
	if (CameraModeStack.Num() > 0)
	{
		return CameraModeStack[0];
	}
	return nullptr;
}

TArray<TObjectPtr<UOasisCameraModeBase>> UOasisCameraModeStack::GetAllActiveMode() const
{
	if (bIsActive)
	{
		return CameraModeStack;
	}
	return {};
}

UOasisCameraModeBase* UOasisCameraModeStack::GetCameraModeInstance(TSubclassOf<UOasisCameraModeBase> CameraModeClass)
{
	check(CameraModeClass);

	// First see if we already created one.
	for (UOasisCameraModeBase* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// Not found, so we need to create it.
	UOasisCameraModeBase* NewCameraMode = NewObject<UOasisCameraModeBase>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);
	NewCameraMode->OnCreated();

	return NewCameraMode;
}

void UOasisCameraModeStack::UpdateStack(FMinimalViewInfo& DefaultCameraView, float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UOasisCameraModeBase* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DefaultCameraView, DeltaTime);

		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			// Everything below this mode is now irrelevant and can be removed.
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// Let the camera modes know they being removed from the stack.
		for (int32 StackIndex = RemoveIndex; StackIndex < StackSize; ++StackIndex)
		{
			UOasisCameraModeBase* CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);

			CameraMode->OnDeactivation();
		}

		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UOasisCameraModeStack::BlendStack(FOasisCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// Start at the bottom and blend up the stack
	const UOasisCameraModeBase* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->GetCameraModeView();

	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
	}
}

void UOasisCameraModeStack::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString(TEXT("   --- Camera Modes (Begin) ---")));

	for (const UOasisCameraModeBase* CameraMode : CameraModeStack)
	{
		check(CameraMode);
		CameraMode->DrawDebug(Canvas);
	}

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   --- Camera Modes (End) ---")));
}

void UOasisCameraModeStack::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutWeightOfTopLayer = 1.0f;
		OutTagOfTopLayer = FGameplayTag();
		return;
	}
	else
	{
		UOasisCameraModeBase* TopEntry = CameraModeStack.Last();
		check(TopEntry);
		OutWeightOfTopLayer = TopEntry->GetBlendWeight();
		OutTagOfTopLayer = TopEntry->GetCameraTypeTag();
	}
}
