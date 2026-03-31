// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "OasisCameraComponentBase.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "OasisCameraModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OasisCameraComponentBase)

UOasisCameraComponentBase::UOasisCameraComponentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOasisCameraComponentBase::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UOasisCameraModeStack>(this);
	}
}

void UOasisCameraComponentBase::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);
	Super::GetCameraView(DeltaTime, DesiredView);
	UpdateCameraModes();

	FOasisCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, DesiredView, CameraModeView);

	// Keep player controller in sync with the latest view.
	if (const APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
			OldControlRotation = CameraModeView.ControlRotation;
		}
	}

	// Apply any offset that was added to the field of view.
	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	// Keep camera component in sync with the latest view.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	OldLocation = CameraModeView.Location;
	OldRotation = CameraModeView.Rotation;
	FieldOfView = CameraModeView.FieldOfView;
	OldFOV = CameraModeView.FieldOfView;
	CameraModeView.ApplyToMinimalView(DesiredView);
}

void UOasisCameraComponentBase::UpdateCameraModes()
{
	check(CameraModeStack);

	if (CameraModeStack->IsStackActivate())
	{
		if (DetermineCameraModeDelegate.IsBound())
		{
			if (const TSubclassOf<UOasisCameraModeBase> CameraMode = DetermineCameraModeDelegate.Execute())
			{
				CameraModeStack->PushCameraMode(CameraMode);
			}
		}
		if (CameraModeStack->IsStackEmpty())
		{
			CameraModeStack->PushCameraMode(DefaultModeClass);
		}
	}
}

void UOasisCameraComponentBase::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);
	check(CameraModeStack);
	CameraModeStack->DrawDebug(Canvas);
}

void UOasisCameraComponentBase::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	check(CameraModeStack);
	CameraModeStack->GetBlendInfo(/*out*/ OutWeightOfTopLayer, /*out*/ OutTagOfTopLayer);
}

bool UOasisCameraComponentBase::TryGetOldControlRotation(FRotator& OutRotator) const
{
	if (OldControlRotation.IsSet())
	{
		OutRotator = OldControlRotation.GetValue();
		return true;
	}
	return false;
}

bool UOasisCameraComponentBase::TryGetOldRotation(FRotator& OutRotator) const
{
	if (OldRotation.IsSet())
	{
		OutRotator = OldRotation.GetValue();
		return true;
	}
	return false;
}

bool UOasisCameraComponentBase::TryGetOldLocation(FVector& OutLocation) const
{
	if (OldLocation.IsSet())
	{
		OutLocation = OldLocation.GetValue();
		return true;
	}
	return false;
}

bool UOasisCameraComponentBase::TryGetOldFOV(float& OutFOV) const
{
	if (OldFOV.IsSet())
	{
		OutFOV = OldFOV.GetValue();
		return true;
	}
	return false;
}

bool UOasisCameraComponentBase::IsStackActive() const
{
	if (!::IsValid(CameraModeStack))
	{
		return false;
	}
	return CameraModeStack->IsStackActivate();
}

bool UOasisCameraComponentBase::IsStackEmpty() const
{
	if (!::IsValid(CameraModeStack))
	{
		return true;
	}
	return CameraModeStack->IsStackEmpty();
}

UOasisCameraModeBase* UOasisCameraComponentBase::GetTopMode() const
{
	if (!::IsValid(CameraModeStack))
	{
		return nullptr;
	}
	return CameraModeStack->GetTopMode();
}

TArray<UOasisCameraModeBase*> UOasisCameraComponentBase::GetAllActiveModes() const
{
	if (!::IsValid(CameraModeStack))
	{
		return {};
	}
	return CameraModeStack->GetAllActiveMode();
}

// From Penguin Assistant Start
void UOasisCameraComponentBase::ChangeTopModeProxyState(const FName& InStateName)
{
	if (::IsValid(CameraModeStack))
	{
		CameraModeStack->ChangeTopModeProxyState(InStateName);
	}
}

UOasisCameraSettingRuntimeDataBase* UOasisCameraComponentBase::GetTopModeSettingRuntimeData(const FName SettingTypeName) const
{
	if (!::IsValid(CameraModeStack))
	{
		return nullptr;
	}
	return CameraModeStack->GetTopModeSettingRuntimeData(SettingTypeName);
}
// From Penguin Assistant End
