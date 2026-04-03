// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "OasisCameraComponentBase.h"
#include "OasisCameraModeBase.h"
#include "OasisCameraType.h"
#include "OasisCamera.h"
#include "CameraSetting/OasisCameraSetting_BasicRotationLimit.h"
#include "CameraSetting/OasisCameraSetting_CollisionTest.h"
#include "CameraSetting/OasisCameraSetting_FollowActorRotation.h"
#include "CameraSetting/OasisCameraSetting_FOV.h"
#include "CameraSetting/OasisCameraSetting_LocationLag.h"
#include "CameraSetting/OasisCameraSetting_RoofCollisionChangeRotationOffset.h"
#include "CameraSetting/OasisCameraSetting_RotationLag.h"
#include "CameraSetting/OasisCameraSetting_RotationOffset.h"
#include "CameraSetting/OasisCameraSetting_SpringArm.h"
// From Penguin Assistant Start
#include "CameraSetting/OasisCameraSetting_LookAt.h"
// From Penguin Assistant End
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

const UOasisCameraSettingBase* UOasisCameraProxyBase::GetReadonlySetting(const FName SettingType) const
{
	return UOasisCameraFunctionLibrary::GetReadonlySettingByProxy(this, SettingType);
}

UOasisCameraSettingRuntimeDataBase* UOasisCameraProxyBase::GetSettingRuntimeData(const FName SettingType) const
{
	return UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(this, SettingType);
}

UOasisCameraModeBase* UOasisCameraFunctionLibrary::GetHostModeByProxy(const UOasisCameraProxyBase* InProxy)
{
	if (!::IsValid(InProxy))
	{
		return nullptr;
	}
	return Cast<UOasisCameraModeBase>(InProxy->GetOuter());
}

UOasisCameraComponentBase* UOasisCameraFunctionLibrary::GetHostComponentByProxy(const UOasisCameraProxyBase* InProxy)
{
	const UOasisCameraModeBase* HostMode = GetHostModeByProxy(InProxy);
	if (!::IsValid(HostMode))
	{
		return nullptr;
	}
	return HostMode->GetOasisCameraComponent();
}

AActor* UOasisCameraFunctionLibrary::GetHostActorByProxy(const UOasisCameraProxyBase* InProxy)
{
	const UOasisCameraComponentBase* HostComponent = GetHostComponentByProxy(InProxy);
	if (!::IsValid(HostComponent))
	{
		return nullptr;
	}
	return HostComponent->GetOwner();
}

APlayerCameraManager* UOasisCameraFunctionLibrary::GetHostCameraManagerByProxy(const UOasisCameraProxyBase* InProxy)
{
	const APawn* HostPawn = Cast<APawn>(GetHostActorByProxy(InProxy));
	if (!::IsValid(HostPawn))
	{
		return nullptr;
	}
	const APlayerController* HostPC = Cast<APlayerController>(HostPawn->GetController());
	if (!::IsValid(HostPC))
	{
		return nullptr;
	}
	return HostPC->PlayerCameraManager;
}

const UOasisCameraSettingBase* UOasisCameraFunctionLibrary::GetReadonlySettingByProxy(const UOasisCameraProxyBase* InProxy, const FName SettingTypeName)
{
	const UOasisCameraModeBase* HostMode = GetHostModeByProxy(InProxy);
	if (!::IsValid(HostMode))
	{
		return nullptr;
	}
	return HostMode->GetReadOnlySetting(SettingTypeName);
}

UOasisCameraSettingRuntimeDataBase* UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(const UOasisCameraProxyBase* InProxy, const FName SettingTypeName)
{
	const UOasisCameraModeBase* HostMode = GetHostModeByProxy(InProxy);
	if (!::IsValid(HostMode))
	{
		return nullptr;
	}
	return HostMode->GetSettingRuntimeData(SettingTypeName);
}


void UOasisCameraFunctionLibrary::OasisCameraModeView_Blend(FOasisCameraModeView& LHS, const FOasisCameraModeView& Other, float OtherWeight)
{
	LHS.Blend(Other, OtherWeight);
}

void UOasisCameraFunctionLibrary::OasisCameraModeView_ApplyToMinimalView(const FOasisCameraModeView& LHS, FMinimalViewInfo& ViewInfo)
{
	LHS.ApplyToMinimalView(ViewInfo);
}

void UOasisCameraFunctionLibrary::OasisCameraModeView_AssignedFromMinimalView(FOasisCameraModeView& LHS, const FMinimalViewInfo& ViewInfo)
{
	LHS.AssignedFromMinimalView(ViewInfo);
}

void UOasisCameraFunctionLibrary::UpdateView_BasicRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	const TWeakObjectPtr<const UOasisCameraSettingBase> Setting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetBasicRotationOffsetSettingTypeName());
	if (!Setting.IsValid())
	{
		return;
	}
	const UOasisCameraSetting_RotationOffset* TypedSetting = Cast<UOasisCameraSetting_RotationOffset>(Setting.Get());
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UpdateView_TryApplyRotationOffsetSetting(InProxy, TypedSetting, DefaultCameraView, DeltaTime, InOutView);
}

void UOasisCameraFunctionLibrary::UpdateView_RoofCollisionChangeRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	const TWeakObjectPtr<const UOasisCameraSettingBase> Setting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetRoofCollisionChangeRotationOffsetSettingTypeName());
	if (!Setting.IsValid())
	{
		return;
	}
	const UOasisCameraSetting_RoofCollisionChangeRotationOffset* TypedSetting = Cast<UOasisCameraSetting_RoofCollisionChangeRotationOffset>(Setting.Get());
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UpdateView_TryApplyRotationOffsetSetting(InProxy, TypedSetting->RotationOffsetSettings, DefaultCameraView, DeltaTime, InOutView);
}

void UOasisCameraFunctionLibrary::UpdateView_FallingChangeRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}

}

void UOasisCameraFunctionLibrary::UpdateView_TryApplyRotationOffsetSetting(const UOasisCameraProxyBase* InProxy, const UOasisCameraSetting_RotationOffset* InSetting, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	if (!::IsValid(InSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_RotationOffset* RuntimeData = Cast<UOasisCameraSettingRuntimeData_RotationOffset>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetBasicRotationOffsetSettingTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	const UOasisCameraComponentBase* HostComponent = UOasisCameraFunctionLibrary::GetHostComponentByProxy(InProxy);
	if (!::IsValid(HostComponent))
	{
		return;
	}
	FRotator OldControlRotation;
	FRotator OldRotator;
	if (!HostComponent->TryGetOldControlRotation(OldControlRotation) || !HostComponent->TryGetOldRotation(OldRotator))
	{
		return;
	}
	bool bMoveCheckResult = true;
	if (InSetting->bActivateOnlyIfMovement)
	{
		const APawn* HostPawn = Cast<APawn>(UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy));
		if (::IsValid(HostPawn))
		{
			bMoveCheckResult = (HostPawn->GetVelocity().SizeSquared() > FMath::Square(InSetting->MinMovementSpeedForActivate));
		}
	}
	const bool bControlRotationPitchChanged = !FMath::IsNearlyEqual(
		FRotator::NormalizeAxis(InOutView.ControlRotation.Pitch),
		FRotator::NormalizeAxis(OldControlRotation.Pitch),
		0.1f);
	const bool bCanAccumulateDeltaTime = !bControlRotationPitchChanged;
	if (bCanAccumulateDeltaTime)
	{
		RuntimeData->CanApplyRotationOffsetAccumulateTime = RuntimeData->CanApplyRotationOffsetAccumulateTime.Get(0.0f) + DeltaTime;
	}
	else
	{
		RuntimeData->CanApplyRotationOffsetAccumulateTime = 0.0f;
	}
	if (RuntimeData->CanApplyRotationOffsetAccumulateTime.GetValue() < InSetting->StartOffsetDelay || !bMoveCheckResult)
	{
		if (RuntimeData->ApplyRotationOffsetInterpolationSpeedInfo.IsSet())
		{
			RuntimeData->ApplyRotationOffsetInterpolationSpeedInfo->ResetCurrentSpeed();
		}
		return;
	}
	if (!RuntimeData->ApplyRotationOffsetInterpolationSpeedInfo.IsSet())
	{
		RuntimeData->ApplyRotationOffsetInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(InSetting->PitchInterpolation);
	}
	TOptional<float> PitchMin;
	TOptional<float> PitchMax;
	TWeakObjectPtr<const UOasisCameraSettingBase> BasicRotationLimitSetting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetBasicRotationLimitSettingTypeName());
	if (BasicRotationLimitSetting.IsValid())
	{
		const UOasisCameraSetting_BasicRotationLimit* TypedBasicRotationLimitSetting = Cast<UOasisCameraSetting_BasicRotationLimit>(BasicRotationLimitSetting.Get());
		if (::IsValid(TypedBasicRotationLimitSetting))
		{
			PitchMin = TypedBasicRotationLimitSetting->ViewPitchMin;
			PitchMax = TypedBasicRotationLimitSetting->ViewPitchMax;
		}
	}
	float TargetPitch = FRotator::NormalizeAxis(InSetting->PitchOffset);
	if (PitchMin.IsSet() && PitchMax.IsSet())
	{
		TargetPitch = FMath::ClampAngle(TargetPitch, PitchMin.GetValue(), PitchMax.GetValue());
	}
	const FRotator TargetRotator(TargetPitch, InOutView.Rotation.Yaw, InOutView.Rotation.Roll);
	const FRotator InterpolatedRotator = UOasisCameraFunctionLibrary::InterpolateRotator(
		OldRotator,
		TargetRotator,
		DeltaTime,
		RuntimeData->ApplyRotationOffsetInterpolationSpeedInfo.GetValue());
	if (InSetting->bPrintPitch)
	{
		UE_LOG(LogOasisCamera, Log, TEXT("target %f, next %f"), TargetPitch, InterpolatedRotator.Pitch);
	}
	InOutView.Rotation.Pitch = InterpolatedRotator.Pitch;
	InOutView.ControlRotation.Pitch = InterpolatedRotator.Pitch;
}

void UOasisCameraFunctionLibrary::UpdateView_FollowActorRotation(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	TWeakObjectPtr<const UOasisCameraSettingBase> Setting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetFollowActorRotationSettingTypeName());
	if (!Setting.IsValid())
	{
		return;
	}
	const UOasisCameraSetting_FollowActorRotation* TypedSetting = Cast<UOasisCameraSetting_FollowActorRotation>(Setting.Get());
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_FollowActorRotation* RuntimeData = Cast<UOasisCameraSettingRuntimeData_FollowActorRotation>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetFollowActorRotationSettingTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	UOasisCameraComponentBase* HostComponent = UOasisCameraFunctionLibrary::GetHostComponentByProxy(InProxy);
	if (!::IsValid(HostComponent))
	{
		return;
	}
	FRotator OldControlRotation;
	FRotator OldRotator;
	if (!HostComponent->TryGetOldControlRotation(OldControlRotation) || !HostComponent->TryGetOldRotation(OldRotator))
	{
		return;
	}
	AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(HostActor))
	{
		return;
	}
	bool bMoveCheckResult = true;
	if (APawn* HostPawn = Cast<APawn>(HostActor))
	{
		bMoveCheckResult = (HostPawn->GetVelocity().SizeSquared() > KINDA_SMALL_NUMBER);
	}
	const float OwningActorYaw = HostActor->GetActorRotation().Yaw;
	const float ActorToViewYawDelta = FMath::Abs(FRotator::NormalizeAxis(OwningActorYaw - InOutView.Rotation.Yaw));
	const bool bYawDeltaCheckResult = (ActorToViewYawDelta < TypedSetting->MaxDeltaForChange);
	const bool bControlRotationYawChanged = !FMath::IsNearlyEqual(
		FRotator::NormalizeAxis(InOutView.ControlRotation.Yaw),
		FRotator::NormalizeAxis(OldControlRotation.Yaw),
		0.1f);
	const bool bCanAccumulateDeltaTime = bYawDeltaCheckResult && !bControlRotationYawChanged;
	if (bCanAccumulateDeltaTime)
	{
		RuntimeData->CanFollowActorRotationAccumulateTime = RuntimeData->CanFollowActorRotationAccumulateTime.Get(0.0f) + DeltaTime;
	}
	else
	{
		RuntimeData->CanFollowActorRotationAccumulateTime = 0.0f;
	}
	if (RuntimeData->CanFollowActorRotationAccumulateTime.GetValue() < TypedSetting->StartChangeDelay || !bMoveCheckResult)
	{
		if (RuntimeData->FollowActorRotationInterpolationSpeedInfo.IsSet())
		{
			RuntimeData->FollowActorRotationInterpolationSpeedInfo->ResetCurrentSpeed();
		}
		return;
	}
	if (!RuntimeData->FollowActorRotationInterpolationSpeedInfo.IsSet())
	{
		RuntimeData->FollowActorRotationInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->RotationInterpolation);
	}
	const float TargetYaw = HostActor->GetActorRotation().Yaw;
	FRotator TargetRotator(OldRotator.Pitch, TargetYaw, OldRotator.Roll);
	if (FMath::IsNearlyEqual(FMath::Abs(FRotator::NormalizeAxis(TargetRotator.Yaw - OldRotator.Yaw)), 180.0f), 0.5f)
	{
		TargetRotator.Yaw -= 1.0f;
	}
	const FRotator InterpolateRotator = UOasisCameraFunctionLibrary::InterpolateRotator(
		OldRotator,
		TargetRotator,
		DeltaTime,
		RuntimeData->FollowActorRotationInterpolationSpeedInfo.GetValue());
	InOutView.Rotation.Yaw = InterpolateRotator.Yaw;
	InOutView.ControlRotation.Yaw = InterpolateRotator.Yaw;
}

void UOasisCameraFunctionLibrary::UpdateView_BasicRotationLimit(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	TWeakObjectPtr<const UOasisCameraSettingBase> Setting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetBasicRotationLimitSettingTypeName());
	if (!Setting.IsValid())
	{
		return;
	}
	const UOasisCameraSetting_BasicRotationLimit* BasicRotationLimitSetting = Cast<UOasisCameraSetting_BasicRotationLimit>(Setting.Get());
	if (!::IsValid(BasicRotationLimitSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_BasicRotationLimit* RuntimeData = Cast<UOasisCameraSettingRuntimeData_BasicRotationLimit>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetBasicRotationLimitSettingTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	if (!RuntimeData->BasicRotationLimitInterpolationSpeedInfo.IsSet())
	{
		RuntimeData->BasicRotationLimitInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(BasicRotationLimitSetting->ViewInterpolation);
	}
	APlayerCameraManager* CameraManager = UOasisCameraFunctionLibrary::GetHostCameraManagerByProxy(InProxy);
	if (!::IsValid(CameraManager))
	{
		InOutView.Rotation.Yaw = FMath::ClampAngle(InOutView.Rotation.Yaw, BasicRotationLimitSetting->ViewYawMin, BasicRotationLimitSetting->ViewYawMax);
		InOutView.Rotation.Pitch = FMath::ClampAngle(InOutView.Rotation.Pitch, BasicRotationLimitSetting->ViewPitchMin, BasicRotationLimitSetting->ViewPitchMax);
		return;
	}
	CameraManager->ViewPitchMin = UOasisCameraFunctionLibrary::InterpolateFloat(
		CameraManager->ViewPitchMin,
		BasicRotationLimitSetting->ViewPitchMin,
		DeltaTime,
		*(RuntimeData->BasicRotationLimitInterpolationSpeedInfo),
		false
	);
	CameraManager->ViewPitchMax = UOasisCameraFunctionLibrary::InterpolateFloat(
		CameraManager->ViewPitchMax,
		BasicRotationLimitSetting->ViewPitchMax,
		DeltaTime,
		*(RuntimeData->BasicRotationLimitInterpolationSpeedInfo),
		false
	);
	CameraManager->ViewYawMin = UOasisCameraFunctionLibrary::InterpolateFloat(
		CameraManager->ViewYawMin,
		BasicRotationLimitSetting->ViewYawMin,
		DeltaTime,
		*(RuntimeData->BasicRotationLimitInterpolationSpeedInfo),
		false
	);
	CameraManager->ViewYawMax = UOasisCameraFunctionLibrary::InterpolateFloat(
		CameraManager->ViewYawMax,
		BasicRotationLimitSetting->ViewYawMax,
		DeltaTime,
		*(RuntimeData->BasicRotationLimitInterpolationSpeedInfo),
		false
	);
	if (FMath::IsNearlyEqual(CameraManager->ViewPitchMin, BasicRotationLimitSetting->ViewPitchMin, BasicRotationLimitSetting->ViewInterpolation.ResetInterpolationSpeedTolerance)
		&& FMath::IsNearlyEqual(CameraManager->ViewPitchMax, BasicRotationLimitSetting->ViewPitchMax, BasicRotationLimitSetting->ViewInterpolation.ResetInterpolationSpeedTolerance)
		&& FMath::IsNearlyEqual(CameraManager->ViewYawMin, BasicRotationLimitSetting->ViewYawMin, BasicRotationLimitSetting->ViewInterpolation.ResetInterpolationSpeedTolerance)
		&& FMath::IsNearlyEqual(CameraManager->ViewYawMax, BasicRotationLimitSetting->ViewYawMax, BasicRotationLimitSetting->ViewInterpolation.ResetInterpolationSpeedTolerance))
	{
		RuntimeData->BasicRotationLimitInterpolationSpeedInfo.GetValue().ResetCurrentSpeed();
	}
}

void UOasisCameraFunctionLibrary::UpdateView_RotationLag(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	const UOasisCameraSetting_RotationLag* TypedSetting = Cast<UOasisCameraSetting_RotationLag>(InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetRotationLagSettingTypeName()));
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_RotationLag* RuntimeData = Cast<UOasisCameraSettingRuntimeData_RotationLag>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetRotationLagSettingTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	if (TypedSetting->CameraLagSpeed <= 0.0f)
	{
		return;
	}
	if (!RuntimeData->OldRotator.IsSet())
	{
		RuntimeData->OldRotator = InOutView.Rotation;
	}
	FRotator OldRotation = RuntimeData->OldRotator.GetValue();
	FRotator TargetRotation = InOutView.Rotation;
	OldRotation.Normalize();
	TargetRotation.Normalize();
	if (TypedSetting->bUseCameraLagIteration && DeltaTime > TypedSetting->CameraLagMaxTimeStep)
	{
		const FRotator RotationStep = (TargetRotation - OldRotation).GetNormalized() * (1.f / DeltaTime);
		FRotator LerpTarget = OldRotation;
		float RemainingTime = DeltaTime;
		while (RemainingTime > KINDA_SMALL_NUMBER)
		{
			const float LerpAmount = FMath::Min(TypedSetting->CameraLagMaxTimeStep, RemainingTime);
			LerpTarget += RotationStep * LerpAmount;
			RemainingTime -= LerpAmount;

			TargetRotation = FRotator(FMath::QInterpTo(FQuat(OldRotation), FQuat(LerpTarget), LerpAmount, TypedSetting->CameraLagSpeed));
			OldRotation = TargetRotation;
		}
	}
	else
	{
		TargetRotation = FRotator(FMath::QInterpTo(FQuat(OldRotation), FQuat(TargetRotation), DeltaTime, TypedSetting->CameraLagSpeed));
	}
	InOutView.Rotation = TargetRotation;
	RuntimeData->OldRotator = TargetRotation;
}

void UOasisCameraFunctionLibrary::UpdateView_SpringArm(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(HostActor))
	{
		return;
	}
	const UOasisCameraSetting_SpringArm* TypedSetting = Cast<UOasisCameraSetting_SpringArm>(
		InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetSpringArmSettingTypeName())
	);
	UOasisCameraSettingRuntimeData_SpringArm* RuntimeData = Cast<UOasisCameraSettingRuntimeData_SpringArm>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetSpringArmSettingTypeName())
	);
	FVector SpringArmStartPosition = HostActor->GetActorLocation();
	if (::IsValid(TypedSetting) && ::IsValid(RuntimeData))
	{
		FVector SpringArmStartWorldOffset = TypedSetting->SpringArmStartWorldOffset;
		if (RuntimeData->OldSpringArmStartOffset.IsSet() && !SpringArmStartWorldOffset.Equals(RuntimeData->OldSpringArmStartOffset.GetValue()))
		{
			if (!RuntimeData->SpringArmStartOffsetInterpolationSpeedInfo.IsSet())
			{
				RuntimeData->SpringArmStartOffsetInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->SpringArmStartOffsetInterpolation);
			}
			SpringArmStartWorldOffset = UOasisCameraFunctionLibrary::InterpolateVector(RuntimeData->OldSpringArmStartOffset.GetValue(), SpringArmStartWorldOffset, DeltaTime, RuntimeData->SpringArmStartOffsetInterpolationSpeedInfo.GetValue());
		}
		RuntimeData->OldSpringArmStartOffset = SpringArmStartWorldOffset;
		SpringArmStartPosition = SpringArmStartPosition + SpringArmStartWorldOffset;
		RuntimeData->TargetSpringArmStartPosition = SpringArmStartPosition;
	}

	UOasisCameraSettingRuntimeData_LocationLag* LocationLagRuntimeData = Cast<UOasisCameraSettingRuntimeData_LocationLag>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetLocationLagSettingTypeName())
	);
	if (::IsValid(LocationLagRuntimeData))
	{
		LocationLagRuntimeData->TargetLocation = SpringArmStartPosition;
		UOasisCameraFunctionLibrary::UpdateView_LocationLag(InProxy, DefaultCameraView, DeltaTime,InOutView);
		SpringArmStartPosition = LocationLagRuntimeData->OldLocation.Get(SpringArmStartPosition);
	}
	if (::IsValid(TypedSetting) && TypedSetting->bDrawDebug)
	{
		DrawDebugSphere(InProxy->GetWorld(), SpringArmStartPosition, 10.0f, 8, FColor::Orange);
	}
	if (::IsValid(RuntimeData))
	{
		RuntimeData->OldSpringArmStartPosition = SpringArmStartPosition;
	}

	UOasisCameraSettingRuntimeData_CollisionTest* CollisionTestRuntimeData = Cast<UOasisCameraSettingRuntimeData_CollisionTest>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetCollisionTestSettingTypeName())
	);
	float SpringArmDistance = 0.0f;
	if (::IsValid(TypedSetting) && ::IsValid(RuntimeData))
	{
		RuntimeData->TargetDistance = TypedSetting->DefaultCameraDistance;
		if (::IsValid(TypedSetting->PitchDistanceCurve.Get()))
		{
			RuntimeData->TargetDistance.GetValue() += TypedSetting->PitchDistanceCurve->GetFloatValue(InOutView.Rotation.Pitch);
		}
		RuntimeData->TargetDistance = FMath::Clamp(RuntimeData->TargetDistance.GetValue(), TypedSetting->MinCameraDistance, TypedSetting->MaxCameraDistance);
		SpringArmDistance = RuntimeData->TargetDistance.GetValue();
	}
	if (::IsValid(TypedSetting) && TypedSetting->bDrawDebug)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Orange, FString::Printf(TEXT("CurrentCameraDistance: %f"), SpringArmDistance));
		}
	}
	if (::IsValid(RuntimeData))
	{
		RuntimeData->OldDistance = SpringArmDistance;
	}

	FVector SpringArmEndPosition = SpringArmStartPosition - InOutView.Rotation.Vector() * SpringArmDistance;
	if (::IsValid(TypedSetting) && TypedSetting->bDrawDebug)
	{
		DrawDebugSphere(InProxy->GetWorld(), SpringArmEndPosition, 10.0f, 8, FColor::Purple);
	}

	FVector SpringArmEndRelativeOffset = FVector::Zero();
	if (::IsValid(RuntimeData))
	{
		RuntimeData->OldSpringArmEndPosition = SpringArmEndPosition;
		if (RuntimeData->OldSpringArmEndOffset.IsSet())
		{
			SpringArmEndRelativeOffset = RuntimeData->OldSpringArmEndOffset.GetValue();
		}
	}

	if (::IsValid(TypedSetting) && ::IsValid(RuntimeData) && !RuntimeData->bPauseUseSpringArmEndOffsetByCurve)
	{
		if (::IsValid(TypedSetting->SpringArmEndRelativeOffsetCurve.Get()))
		{
			RuntimeData->TargetSpringArmEndOffset = TypedSetting->SpringArmEndRelativeOffsetCurve->GetVectorValue(SpringArmDistance);
		}
		if (RuntimeData->OldSpringArmEndOffset.IsSet() && !RuntimeData->TargetSpringArmEndOffset.Get(FVector::Zero()).Equals(RuntimeData->OldSpringArmEndOffset.GetValue()))
		{
			if (!RuntimeData->SpringArmEndOffsetInterpolationSpeedInfo.IsSet())
			{
				RuntimeData->SpringArmEndOffsetInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->SpringArmEndOffsetInterpolation);
			}
			SpringArmEndRelativeOffset = UOasisCameraFunctionLibrary::InterpolateVector(
				RuntimeData->OldSpringArmEndOffset.GetValue(),
				RuntimeData->TargetSpringArmEndOffset.Get(FVector::Zero()),
				DeltaTime,
				RuntimeData->SpringArmEndOffsetInterpolationSpeedInfo.GetValue());
		}
	}
	if (::IsValid(RuntimeData))
	{
		RuntimeData->OldSpringArmEndOffset = SpringArmEndRelativeOffset;
	}

	SpringArmEndPosition = SpringArmEndPosition + FRotationMatrix(InOutView.Rotation).TransformVector(SpringArmEndRelativeOffset);
	if (::IsValid(TypedSetting) && TypedSetting->bDrawDebug)
	{
		DrawDebugSphere(InProxy->GetWorld(), SpringArmEndPosition, 10.0f, 8, FColor::Magenta);
	}
	InOutView.Location = SpringArmEndPosition;
}

void UOasisCameraFunctionLibrary::UpdateView_LocationLag(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	const UOasisCameraSetting_LocationLag* TypedSetting = Cast<UOasisCameraSetting_LocationLag>(InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetLocationLagSettingTypeName()));
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_LocationLag* RuntimeData = Cast<UOasisCameraSettingRuntimeData_LocationLag>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetLocationLagSettingTypeName())
	);
	if (!::IsValid(RuntimeData) || !RuntimeData->TargetLocation.IsSet())
	{
		return;
	}
	if (!RuntimeData->OldLocation.IsSet())
	{
		RuntimeData->OldLocation = RuntimeData->TargetLocation;
	}
	if (TypedSetting->CameraLagSpeedXY <= 0.0f && TypedSetting->CameraLagSpeedZ <= 0.0f)
	{
		return;
	}

	bool bClampedDist = false;
	const FVector TargetLocation = RuntimeData->TargetLocation.GetValue();
	if (TypedSetting->CameraLagSpeedXY > 0.0f)
	{
		FVector OldLocationXY = RuntimeData->OldLocation.GetValue();
		OldLocationXY.Z = 0.0f;
		FVector TargetLocationXY = RuntimeData->TargetLocation.GetValue();
		TargetLocationXY.Z = 0.0f;
		FVector InterpolatedLocationXY = TargetLocationXY;
		if (DeltaTime > TypedSetting->CameraLagMaxTimeStep)
		{
			const FVector MovementStepXY = (TargetLocationXY - OldLocationXY) * (1.0 / DeltaTime);
			FVector LerpTargetXY = OldLocationXY;
			float RemainingTimeXY = DeltaTime;
			while (RemainingTimeXY > KINDA_SMALL_NUMBER)
			{
				const float LerpAmountXY = FMath::Min(TypedSetting->CameraLagMaxTimeStep, RemainingTimeXY);
				LerpTargetXY += MovementStepXY * LerpAmountXY;
				RemainingTimeXY -= LerpAmountXY;
				InterpolatedLocationXY = FMath::VInterpTo(OldLocationXY, LerpTargetXY, LerpAmountXY, TypedSetting->CameraLagSpeedXY);
			}
		}
		else
		{
			InterpolatedLocationXY = FMath::VInterpTo(OldLocationXY, TargetLocationXY, DeltaTime, TypedSetting->CameraLagSpeedXY);
		}
		if (TypedSetting->CameraLagMaxDistanceXY > 0.f)
		{
			const FVector DeltaVector = InterpolatedLocationXY - TargetLocationXY;
			if (DeltaVector.SizeSquared() > FMath::Square(TypedSetting->CameraLagMaxDistanceXY))
			{
				InterpolatedLocationXY = TargetLocationXY + DeltaVector.GetClampedToMaxSize(TypedSetting->CameraLagMaxDistanceXY);
				bClampedDist = true;
			}
		}
		RuntimeData->OldLocation.GetValue().X = InterpolatedLocationXY.X;
		RuntimeData->OldLocation.GetValue().Y = InterpolatedLocationXY.Y;
	}
	if (TypedSetting->CameraLagSpeedZ > 0.0f)
	{
		float OldLocationZ = RuntimeData->OldLocation.GetValue().Z;
		float TargetLocationZ = RuntimeData->TargetLocation.GetValue().Z;
		float InterpolatedLocationZ = TargetLocationZ;
		if (DeltaTime > TypedSetting->CameraLagMaxTimeStep)
		{

			const float MovementStepZ = (TargetLocationZ - OldLocationZ) * (1.0 / DeltaTime);
			float LerpTargetZ = OldLocationZ;
			float RemainingTimeZ = DeltaTime;
			while (RemainingTimeZ > KINDA_SMALL_NUMBER)
			{
				const float LerpAmountZ = FMath::Min(TypedSetting->CameraLagMaxTimeStep, RemainingTimeZ);
				LerpTargetZ += MovementStepZ * LerpAmountZ;
				RemainingTimeZ -= LerpAmountZ;
				InterpolatedLocationZ = FMath::FInterpTo(OldLocationZ, LerpTargetZ, LerpAmountZ, TypedSetting->CameraLagSpeedZ);
			}
		}
		else
		{
			InterpolatedLocationZ = FMath::FInterpTo(OldLocationZ, TargetLocationZ, DeltaTime, TypedSetting->CameraLagSpeedZ);
		}
		if (TypedSetting->CameraLagMaxDistanceZ > 0.f)
		{
			const float Delta = TargetLocationZ - InterpolatedLocationZ;
			if (FMath::Abs(Delta) > TypedSetting->CameraLagMaxDistanceZ)
			{
				InterpolatedLocationZ = TargetLocationZ - FMath::Sign(Delta) * TypedSetting->CameraLagMaxDistanceZ;
				bClampedDist = true;
			}
		}
		RuntimeData->OldLocation.GetValue().Z = InterpolatedLocationZ;
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (TypedSetting->bDrawDebugLagMarkers)
	{
		FVector OldLocation = RuntimeData->OldLocation.GetValue();
		DrawDebugSphere(InProxy->GetWorld(), OldLocation, 5.f, 8, FColor::Green);
		DrawDebugSphere(InProxy->GetWorld(), TargetLocation, 5.f, 8, FColor::Yellow);

		const FVector ToOrigin = TargetLocation - OldLocation;
		DrawDebugDirectionalArrow(InProxy->GetWorld(), OldLocation, OldLocation + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
		DrawDebugDirectionalArrow(InProxy->GetWorld(), OldLocation + ToOrigin * 0.5f, TargetLocation, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
	}
#endif
}

void UOasisCameraFunctionLibrary::UpdateView_CollisionTest(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	// necessary info check and get
	if (!::IsValid(InProxy))
	{
		return;
	}
	const UOasisCameraSetting_CollisionTest* TypedSetting =
		Cast<UOasisCameraSetting_CollisionTest>(InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetCollisionTestSettingTypeName()));
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_CollisionTest* RuntimeData = Cast<UOasisCameraSettingRuntimeData_CollisionTest>(
			UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetCollisionTestSettingTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	UOasisCameraComponentBase* HostComponent = UOasisCameraFunctionLibrary::GetHostComponentByProxy(InProxy);
	if (!::IsValid(HostComponent))
	{
		return;
	}
	AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(HostActor))
	{
		return;
	}

	// calculate collision trace info
	UOasisCameraSettingRuntimeData_SpringArm* SpringArmSettingRuntimeData = Cast<UOasisCameraSettingRuntimeData_SpringArm>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetSpringArmSettingTypeName())
	);
	FVector InFocusPoint = HostActor->GetActorLocation();
	if (::IsValid(SpringArmSettingRuntimeData) && SpringArmSettingRuntimeData->OldSpringArmStartPosition.IsSet())
	{
		InFocusPoint = SpringArmSettingRuntimeData->OldSpringArmStartPosition.GetValue();
	}
	const FVector FocusPoint = InFocusPoint;
	const FVector TraceEnd = InOutView.Location;

	// do collision trace
	FHitResult SphereHitResult;
	FHitResult ConeHitInnerResult;
	FHitResult ConeHitOuterResult;
	FRotator OldControlRotation = InOutView.ControlRotation;
	HostComponent->TryGetOldControlRotation(OldControlRotation);
	FRotator NewControlRotation = InOutView.ControlRotation;
	SphereCollisionTest(InProxy, SphereHitResult, TypedSetting, FocusPoint, TraceEnd);
	if (!SphereHitResult.bBlockingHit)
	{
		ConeCollisionTest(InProxy, ConeHitInnerResult, ConeHitOuterResult, TypedSetting, FocusPoint, TraceEnd);
	}

	// get correction info
	ProcessTargetCollisionCorrectionPosition(InProxy, SphereHitResult, ConeHitInnerResult, TypedSetting, RuntimeData, InOutView.Location, FocusPoint, TraceEnd);
	FVector InPositionAfterCollisionCorrection;
	const bool bHasPositionAfterCollisionCorrection = GetPositionAfterCollisionCorrection(InProxy, InPositionAfterCollisionCorrection, TypedSetting, RuntimeData, InOutView.Location, DeltaTime);
	FVector AfterCorrectionPosition = bHasPositionAfterCollisionCorrection? InPositionAfterCollisionCorrection : TraceEnd;
	FVector InPositionAfterCollisionCorrectionRecover = FVector::Zero();
	const bool bHasPositionAfterCollisionCorrectionRecover = GetPositionAfterCollisionCorrectionRecover(InProxy, TypedSetting, RuntimeData, InPositionAfterCollisionCorrectionRecover, FocusPoint, AfterCorrectionPosition, OldControlRotation, NewControlRotation, DeltaTime);
	if (bHasPositionAfterCollisionCorrectionRecover)
	{
		AfterCorrectionPosition = InPositionAfterCollisionCorrectionRecover;
	}

	// apply correction
	if (bHasPositionAfterCollisionCorrection || bHasPositionAfterCollisionCorrectionRecover)
	{
		InOutView.Location = AfterCorrectionPosition;
	}
	RuntimeData->OldCollisionCorrectionPosition = AfterCorrectionPosition;
}

void UOasisCameraFunctionLibrary::UpdateView_FOV(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	const UOasisCameraSetting_FOV* TypedSetting = Cast<UOasisCameraSetting_FOV>(InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetFOVSettingAssetTypeName()));
	if (!::IsValid(TypedSetting))
	{
		return;
	}
	UOasisCameraSettingRuntimeData_FOV* RuntimeData = Cast<UOasisCameraSettingRuntimeData_FOV>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetFOVSettingAssetTypeName())
	);
	if (!::IsValid(RuntimeData))
	{
		return;
	}
	float FinalFOV = TypedSetting->BaseCameraFOV;
	if (::IsValid(TypedSetting->FOVByDistanceOfCharacterCurve.Get()))
	{
		UOasisCameraComponentBase* HostComponent = UOasisCameraFunctionLibrary::GetHostComponentByProxy(InProxy);
		AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
		if (::IsValid(HostComponent) && ::IsValid(HostActor))
		{
			float Distance = FVector::Distance(HostComponent->GetComponentLocation(), HostActor->GetActorLocation());
			FinalFOV += TypedSetting->FOVByDistanceOfCharacterCurve->GetFloatValue(Distance);
		}
	}
	if (::IsValid(TypedSetting->FOVByMovementSpeedCurve.Get()))
	{
		APawn* HostPawn = Cast<APawn>(UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy));
		if (::IsValid(HostPawn))
		{
			float Speed = HostPawn->GetVelocity().Size();
			FinalFOV += TypedSetting->FOVByMovementSpeedCurve->GetFloatValue(Speed);
		}
	}
	if (RuntimeData->OldFOV.IsSet())
	{
		if (!RuntimeData->RuntimeFOVInterpolateInfo.IsSet())
		{
			RuntimeData->RuntimeFOVInterpolateInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->FOVInterpolation);
		}
		FinalFOV = UOasisCameraFunctionLibrary::InterpolateFloat(RuntimeData->OldFOV.GetValue(), FinalFOV, DeltaTime, RuntimeData->RuntimeFOVInterpolateInfo.GetValue());
	}
	InOutView.FieldOfView = FinalFOV;
	RuntimeData->OldFOV = FinalFOV;
}

bool UOasisCameraFunctionLibrary::SphereCollisionTest(const UOasisCameraProxyBase* InProxy, FHitResult& OutResult, const UOasisCameraSetting_CollisionTest* TypedSetting, const FVector& FocusPoint, const FVector& TraceEnd)
{
	if (!::IsValid(InProxy))
	{
		return false;
	}
	AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(HostActor) || !::IsValid(TypedSetting))
	{
		return false;
	}
	bool bMoveCheckResult = false;
	if (APawn* HostPawn = Cast<APawn>(HostActor))
	{
		bMoveCheckResult = (HostPawn->GetVelocity().SizeSquared() > KINDA_SMALL_NUMBER);
	}
	EDrawDebugTrace::Type TraceDebugType = TypedSetting->bDrawDebug ? EDrawDebugTrace::Type::ForOneFrame : EDrawDebugTrace::Type::None;
	if (TypedSetting->bDoMovementCollisionTest && bMoveCheckResult)
	{
		TArray<FHitResult> SphereHitResults;
		UKismetSystemLibrary::SphereTraceMulti(
				HostActor,
				FocusPoint,
				TraceEnd,
				TypedSetting->SphereRadius,
				UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
				false,
				{},
				TraceDebugType,
				SphereHitResults,
				true,
				FLinearColor::White,
				FLinearColor::Black);
		for (const FHitResult& HitResult : SphereHitResults)
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			if (::IsValid(HitComponent))
			{
				if (HitComponent->ComponentHasTag(TypedSetting->MovementCollisionTestCollisionTag))
				{
					continue;
				}
				else if (HitComponent->GetCollisionResponseToChannel(TypedSetting->ProbeChannel) == ECollisionResponse::ECR_Block)
				{
					OutResult = HitResult;
					OutResult.bBlockingHit = true;
					return true;
				}
			}
		}
	}
	else
	{
		FHitResult SphereHitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			HostActor,
			FocusPoint,
			TraceEnd,
			TypedSetting->SphereRadius,
			UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
			false,
			{},
			TraceDebugType,
			SphereHitResult,
			true,
			FLinearColor::White,
			FLinearColor::Black);
		if (SphereHitResult.bBlockingHit)
		{
			OutResult = SphereHitResult;
			return true;
		}
	}
	return false;
}

bool UOasisCameraFunctionLibrary::ConeCollisionTest(const UOasisCameraProxyBase* InProxy, FHitResult& InnerResult, FHitResult& OuterResult, const UOasisCameraSetting_CollisionTest* TypedSetting, const FVector& FocusPoint, const FVector& TraceEnd)
{
	if (!::IsValid(InProxy))
	{
		return false;
	}
	AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(HostActor) || !::IsValid(TypedSetting))
	{
		return false;
	}
	if (TypedSetting->ConeStepAngle <= 0.0f)
	{
		return false;
	};
	bool bMoveCheckResult = false;
	if (APawn* HostPawn = Cast<APawn>(HostActor))
	{
		bMoveCheckResult = (HostPawn->GetVelocity().SizeSquared() > KINDA_SMALL_NUMBER);
	}
	if (TypedSetting->bIgnoreConeCollisionWhenMove && bMoveCheckResult)
	{
		return false;
	}
	EDrawDebugTrace::Type TraceDebugType = TypedSetting->bDrawDebug ? EDrawDebugTrace::Type::ForOneFrame : EDrawDebugTrace::Type::None;
	const float Distance = (FocusPoint - TraceEnd).Size();
	const float CollisionTestConeInnerRadius = UKismetMathLibrary::DegTan(TypedSetting->InnerLayerAngle) * Distance;
	const float CollisionTestConeOuterRadius = UKismetMathLibrary::DegTan(TypedSetting->OuterLayerAngle) * Distance;
	float StepAngle = 0.0f;
	const FTransform CollisionTraceEndBaseTransform((FocusPoint - TraceEnd).ToOrientationQuat(), TraceEnd);
	while (StepAngle <= 361.0f)
	{
		FVector InnerRelativeTraceStart(Distance, CollisionTestConeInnerRadius * UKismetMathLibrary::DegCos(StepAngle),
		                                CollisionTestConeInnerRadius * UKismetMathLibrary::DegSin(StepAngle));
		FVector InnerWorldTraceStart = CollisionTraceEndBaseTransform.TransformPosition(InnerRelativeTraceStart);
		FVector OuterRelativeTraceStart(Distance, CollisionTestConeOuterRadius * UKismetMathLibrary::DegCos(StepAngle),
		                                CollisionTestConeOuterRadius * UKismetMathLibrary::DegSin(StepAngle));
		FVector OuterWorldTraceStart = CollisionTraceEndBaseTransform.TransformPosition(OuterRelativeTraceStart);
		FHitResult OuterStartSphereHitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			HostActor,
			OuterWorldTraceStart,
			OuterWorldTraceStart,
			TypedSetting->SphereRadius,
			UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
			false,
			{},
			TraceDebugType,
			OuterStartSphereHitResult,
			true,
			FLinearColor::Blue,
			FLinearColor::Yellow);
		if (OuterStartSphereHitResult.bBlockingHit)
		{
			break;
		}
		FHitResult InnerStartSphereHitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			HostActor,
			InnerWorldTraceStart,
			InnerWorldTraceStart,
			TypedSetting->SphereRadius,
			UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
			false,
			{},
			TraceDebugType,
			InnerStartSphereHitResult,
			true,
			FLinearColor::Green,
			FLinearColor::Red);
		if (InnerStartSphereHitResult.bBlockingHit)
		{
			break;
		}
		if (TypedSetting->bDoMovementCollisionTest && bMoveCheckResult)
		{
			FHitResult InnerHitResult;
			FHitResult OuterHitResult;
			TArray<FHitResult> InnerHitResults;
			TArray<FHitResult> OuterHitResults;
			UKismetSystemLibrary::LineTraceMulti(
				HostActor,
				InnerWorldTraceStart,
				TraceEnd,
				UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
				false,
				{},
				TraceDebugType,
				InnerHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red);
			for (const FHitResult& HitResult : InnerHitResults)
			{
				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (::IsValid(HitComponent))
				{
					if (HitComponent->ComponentHasTag(TypedSetting->MovementCollisionTestCollisionTag))
					{
						continue;
					}
					else if (HitComponent->GetCollisionResponseToChannel(TypedSetting->ProbeChannel) == ECollisionResponse::ECR_Block)
					{
						InnerHitResult = HitResult;
						InnerHitResult.bBlockingHit = true;
						InnerResult = InnerHitResult;
						break;
					}
				}
			}
			UKismetSystemLibrary::LineTraceMulti(
				HostActor,
				OuterWorldTraceStart,
				TraceEnd,
				UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
				false,
				{},
				TraceDebugType,
				OuterHitResults,
				true,
				FLinearColor::Blue,
				FLinearColor::Yellow);
			for (const FHitResult& HitResult : OuterHitResults)
			{
				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (::IsValid(HitComponent))
				{
					if (HitComponent->ComponentHasTag(TypedSetting->MovementCollisionTestCollisionTag))
					{
						continue;
					}
					else if (HitComponent->GetCollisionResponseToChannel(TypedSetting->ProbeChannel) == ECollisionResponse::ECR_Block)
					{
						OuterHitResult = HitResult;
						OuterHitResult.bBlockingHit = true;
						OuterResult = OuterHitResult;
						break;
					}
				}
			}
			if (InnerHitResult.bBlockingHit && OuterHitResult.bBlockingHit)
			{
				return true;
			}
		}
		else
		{
			FHitResult InnerHitResult;
			FHitResult OuterHitResult;
			UKismetSystemLibrary::LineTraceSingle(
				HostActor,
				InnerWorldTraceStart,
				TraceEnd,
				UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
				false,
				{},
				TraceDebugType,
				InnerHitResult,
				true,
				FLinearColor::Green,
				FLinearColor::Red);
			UKismetSystemLibrary::LineTraceSingle(
				HostActor,
				OuterWorldTraceStart,
				TraceEnd,
				UEngineTypes::ConvertToTraceType(TypedSetting->ProbeChannel),
				false,
				{},
				TraceDebugType,
				OuterHitResult,
				true,
				FLinearColor::Blue,
				FLinearColor::Yellow);
			InnerResult = InnerHitResult;
			OuterResult = OuterHitResult;
			if (InnerHitResult.bBlockingHit && OuterHitResult.bBlockingHit)
			{
				return true;
			}
		}
		StepAngle += TypedSetting->ConeStepAngle;
	}
	return false;
}

void UOasisCameraFunctionLibrary::ProcessTargetCollisionCorrectionPosition(const UOasisCameraProxyBase* InProxy, const FHitResult& SphereHitResult, const FHitResult& ConeHitResult, const UOasisCameraSetting_CollisionTest* TypedSetting, UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, const FVector& LocationWithoutCorrection, const FVector& FocusPoint, const FVector& TraceEnd)
{
	if (!::IsValid(InProxy))
	{
		return;
	}
	if (!::IsValid(TypedSetting) || !::IsValid(TypedRuntimeData))
	{
		return;
	}
	if (SphereHitResult.bBlockingHit)
	{
		TypedRuntimeData->TargetCollisionCorrectionPosition = SphereHitResult.Location;
		TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo.Reset();
		TypedRuntimeData->RelativeTargetCollisionCorrectionPosition.Reset();
		TypedRuntimeData->RelativeOldCollisionCorrectionPosition.Reset();
	}
	else if (ConeHitResult.bBlockingHit)
	{
		if (!TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo.IsSet())
		{
			TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(
				TypedSetting->CollisionCorrectionInterpolationSpeedInfo
			);
		}

		const FVector ConeTraceVector = ConeHitResult.Location - FocusPoint;
		const FVector SphereTraceVector = TraceEnd - FocusPoint;
		const FVector ResultVector = UKismetMathLibrary::ProjectVectorOnToVector(ConeTraceVector, SphereTraceVector);
		TypedRuntimeData->TargetCollisionCorrectionPosition = FocusPoint + ResultVector;
		const AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
		if (::IsValid(HostActor))
		{
			TypedRuntimeData->RelativeTargetCollisionCorrectionPosition = HostActor->GetActorTransform().InverseTransformPosition(TypedRuntimeData->TargetCollisionCorrectionPosition.GetValue());
			if (!TypedRuntimeData->RelativeOldCollisionCorrectionPosition.IsSet())
			{
				TypedRuntimeData->RelativeOldCollisionCorrectionPosition = HostActor->GetActorTransform().InverseTransformPosition(LocationWithoutCorrection);
			}
		}
	}
	else
	{
		TypedRuntimeData->TargetCollisionCorrectionPosition.Reset();
		TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo.Reset();
		TypedRuntimeData->RelativeTargetCollisionCorrectionPosition.Reset();
		TypedRuntimeData->RelativeOldCollisionCorrectionPosition.Reset();
	}
}

bool UOasisCameraFunctionLibrary::GetPositionAfterCollisionCorrection(const UOasisCameraProxyBase* InProxy, FVector& OutVector, const UOasisCameraSetting_CollisionTest* TypedSetting, UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, const FVector& CurrentLocation, const float DeltaTime)
{
	TOptional<FVector> OutResult;
	if (!::IsValid(InProxy))
	{
		return OutResult.IsSet();
	}
	if (!::IsValid(TypedSetting) || !::IsValid(TypedRuntimeData))
	{
		return OutResult.IsSet();
	}
	if (TypedRuntimeData->TargetCollisionCorrectionPosition.IsSet())
	{
		if (TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo.IsSet()
		&& TypedRuntimeData->RelativeTargetCollisionCorrectionPosition.IsSet()
		&& TypedRuntimeData->RelativeOldCollisionCorrectionPosition.IsSet())
		{
			const FVector RelativeResult = UOasisCameraFunctionLibrary::InterpolateVector(
				TypedRuntimeData->RelativeOldCollisionCorrectionPosition.GetValue(),
				TypedRuntimeData->RelativeTargetCollisionCorrectionPosition.GetValue(),
				DeltaTime,
				TypedRuntimeData->CollisionCorrectionInterpolationSpeedInfo.GetValue());
			TypedRuntimeData->RelativeOldCollisionCorrectionPosition = RelativeResult;
			const AActor* HostActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
			if (::IsValid(HostActor))
			{
				OutResult = HostActor->GetActorTransform().TransformPosition(RelativeResult);
			}
		}
		else
		{
			OutResult = TypedRuntimeData->TargetCollisionCorrectionPosition.GetValue();
		}
	}
	if (OutResult.IsSet())
	{
		OutVector = OutResult.GetValue();
	}
	return OutResult.IsSet();
}

bool UOasisCameraFunctionLibrary::GetPositionAfterCollisionCorrectionRecover(const UOasisCameraProxyBase* InProxy, const UOasisCameraSetting_CollisionTest* TypedSetting, UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, FVector& OutVector, const FVector& FocusPoint, const FVector& PositionAfterCorrection, const FRotator& OldControlRotation, const FRotator& NewControlRotation, const float DeltaTime)
{
	if (!::IsValid(InProxy) || !::IsValid(TypedSetting) || !::IsValid(TypedRuntimeData))
	{
		return false;
	}
	const float NewTraceDistance = UKismetMathLibrary::Vector_Distance(FocusPoint, PositionAfterCorrection);
	if (!TypedRuntimeData->OldCollisionTraceDistance.IsSet()
		|| NewTraceDistance - TypedRuntimeData->OldCollisionTraceDistance.GetValue() <= TypedSetting->DistanceOffsetOfImmediatelyApplyNewCorrectedLocation)
	{
		TypedRuntimeData->OldCollisionTraceDistance = NewTraceDistance;
		TypedRuntimeData->RecoverCollisionAffectedDistanceInterpolationSpeedInfo.Reset();
		TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.Reset();
		return false;
	}
	bool bCanAccumulateRecoverDelayTime = true;
	bool bCanRecover = true;
	const bool bControlRotationPitchChanged = !FMath::IsNearlyEqual(
			FRotator::NormalizeAxis(NewControlRotation.Pitch),
			FRotator::NormalizeAxis(OldControlRotation.Pitch),
			0.1f);
	const bool bControlRotationYawChanged = !FMath::IsNearlyEqual(
		FRotator::NormalizeAxis(NewControlRotation.Yaw),
		FRotator::NormalizeAxis(OldControlRotation.Yaw),
		0.1f);
	UOasisCameraSettingRuntimeData_SpringArm* SpringArmSettingRuntimeData = Cast<UOasisCameraSettingRuntimeData_SpringArm>(GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetSpringArmSettingTypeName()));
	if ((bControlRotationPitchChanged || bControlRotationYawChanged) && (::IsValid(SpringArmSettingRuntimeData) && NewTraceDistance <= SpringArmSettingRuntimeData->TargetDistance.Get(0.0f) + TypedSetting->BlockCollisionAffectedDistanceRecoverWhenControllerRotationChangedTraceDistanceOffset))
	{
		bCanAccumulateRecoverDelayTime = false;
	}
	if (bCanAccumulateRecoverDelayTime)
	{
		if (!TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.IsSet())
		{
			TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime = 0.0f;
		}
		TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime = TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.GetValue() + DeltaTime;
	}
	else
	{
		TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.Reset();
	}
	if (!TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.IsSet() || TypedRuntimeData->CanRecoverCollisionAffectedDistanceAccumulateTime.GetValue() < TypedSetting->CollisionAffectedDistanceRecoverDelayTime)
	{
		bCanRecover = false;
	}
	if (bCanRecover)
	{
		if (!TypedRuntimeData->RecoverCollisionAffectedDistanceInterpolationSpeedInfo.IsSet())
		{
			TypedRuntimeData->RecoverCollisionAffectedDistanceInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->CollisionAffectedDistanceRecoverInterpolationSpeedInfo);
		}
		const float RecoveredTraceDistance = InterpolateFloat(TypedRuntimeData->OldCollisionTraceDistance.GetValue(), NewTraceDistance, DeltaTime, TypedRuntimeData->RecoverCollisionAffectedDistanceInterpolationSpeedInfo.GetValue());
		if (FMath::IsNearlyEqual(RecoveredTraceDistance, NewTraceDistance, 0.1f))
		{
			TypedRuntimeData->OldCollisionTraceDistance.Reset();
			return false;
		}
		OutVector = FocusPoint + (PositionAfterCorrection - FocusPoint).GetSafeNormal() * RecoveredTraceDistance;
		TypedRuntimeData->OldCollisionTraceDistance = RecoveredTraceDistance;
		return true;
	}
	else
	{
		OutVector = FocusPoint + (PositionAfterCorrection - FocusPoint).GetSafeNormal() * TypedRuntimeData->OldCollisionTraceDistance.GetValue();
		return true;
	}

}

bool UOasisCameraFunctionLibrary::CanRoofCollisionChangeRotationOffset(const UOasisCameraProxyBase* InProxy)
{
	if (!::IsValid(InProxy))
	{
		return false;
	}
	const TWeakObjectPtr<const UOasisCameraSettingBase> Setting = InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetRoofCollisionChangeRotationOffsetSettingTypeName());
	if (!Setting.IsValid())
	{
		return false;
	}
	const UOasisCameraSetting_RoofCollisionChangeRotationOffset* TypedSetting = Cast<UOasisCameraSetting_RoofCollisionChangeRotationOffset>(Setting.Get());
	if (!::IsValid(TypedSetting))
	{
		return false;
	}
	const AActor* OwningActor = UOasisCameraFunctionLibrary::GetHostActorByProxy(InProxy);
	if (!::IsValid(OwningActor))
	{
		return false;
	}
	FVector ActorViewPoint;
	FRotator ActorViewRotator;
	OwningActor->GetActorEyesViewPoint(ActorViewPoint, ActorViewRotator);

	FHitResult EyesHit;
	const FVector EyesTraceStart = ActorViewPoint + OwningActor->GetActorRotation().Vector() * TypedSetting->EyesBoxTraceHalfSize.X;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		InProxy,
		EyesTraceStart,
		EyesTraceStart + 1.f,
		TypedSetting->EyesBoxTraceHalfSize,
		OwningActor->GetActorRotation(),
		TypedSetting->TraceObjectTypes,
		false,
		{},
		EDrawDebugTrace::None,
		EyesHit,
		true);

	FHitResult ActorFirstRoofHit;
	const FRotator ActorRoofTraceRotation = FRotator(TypedSetting->RoofTraceAngleFirst, 0.f, 0.f) + OwningActor->GetActorRotation();
	const FVector ActorRoofTraceStart = ActorViewPoint + ActorRoofTraceRotation.Vector() * TypedSetting->RoofBoxTraceHalfSizeFirst.X;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		InProxy,
		ActorRoofTraceStart,
		ActorRoofTraceStart + 1.f,
		TypedSetting->RoofBoxTraceHalfSizeFirst,
		ActorRoofTraceRotation,
		TypedSetting->TraceObjectTypes,
		false,
		{},
		EDrawDebugTrace::None,
		ActorFirstRoofHit,
		true);

	FHitResult ActorSecondRoofHit;
	const FRotator CameraRoofTraceRotation = FRotator(TypedSetting->RoofTraceAngleSecond, 0.f, 0.f) + OwningActor->GetActorRotation();
	const FVector CameraRoofTraceStart = ActorViewPoint + CameraRoofTraceRotation.Vector() * TypedSetting->RoofBoxTraceHalfSizeSecond.X;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		InProxy,
		CameraRoofTraceStart,
		CameraRoofTraceStart + 1.f,
		TypedSetting->RoofBoxTraceHalfSizeSecond,
		CameraRoofTraceRotation,
		TypedSetting->TraceObjectTypes,
		false,
		{},
		EDrawDebugTrace::None,
		ActorSecondRoofHit,
		true);

	if (EyesHit.bBlockingHit && (ActorFirstRoofHit.bBlockingHit || ActorSecondRoofHit.bBlockingHit))
	{
		return true;
	}
	return false;
}

// From Penguin Assistant Start
void UOasisCameraFunctionLibrary::UpdateView_LookAt(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (!::IsValid(InProxy))
	{
		return;
	}

	const UOasisCameraSetting_LookAt* TypedSetting = Cast<UOasisCameraSetting_LookAt>(
		InProxy->GetReadonlySetting(UOasisCameraSettingTypeDictionary::GetLookAtSettingTypeName())
	);
	UOasisCameraSettingRuntimeData_LookAt* RuntimeData = Cast<UOasisCameraSettingRuntimeData_LookAt>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(InProxy, UOasisCameraSettingTypeDictionary::GetLookAtSettingTypeName())
	);

	if (!::IsValid(RuntimeData))
	{
		return;
	}

	// Initialize runtime data with setting values
	if (!RuntimeData->LockTargetPoint.IsSet() && ::IsValid(TypedSetting))
	{
		RuntimeData->LockTargetPoint = TypedSetting->DefaultLockTargetPoint;
	}

	if (::IsValid(TypedSetting))
	{
		RuntimeData->bIsLockEnabled = TypedSetting->bEnableLock;
	}

	// Check if lock is enabled
	if (!RuntimeData->bIsLockEnabled)
	{
		return;
	}

	// Get lock target point
	if (!RuntimeData->LockTargetPoint.IsSet())
	{
		return;
	}

	const FVector TargetPoint = RuntimeData->LockTargetPoint.GetValue();

	// Calculate target rotation (direction from camera location to target point)
	const FVector DirectionToTarget = (TargetPoint - InOutView.Location).GetSafeNormal();
	FRotator TargetLookRotation = DirectionToTarget.Rotation();
	TargetLookRotation.Roll = 0.f;
	TargetLookRotation.Normalize();

	// Initialize interpolation speed info if needed
	if (::IsValid(TypedSetting) && !RuntimeData->TargetRotationInterpolationSpeedInfo.IsSet())
	{
		RuntimeData->TargetRotationInterpolationSpeedInfo = FRuntimeOasisCameraInterpolationSpeedInfo(TypedSetting->RotationInterpolation);
	}

	// Initialize current rotation if needed
	if (!RuntimeData->CurrentLookRotation.IsSet())
	{
		FRotator InitialLookRotation = InOutView.Rotation;
		InitialLookRotation.Roll = 0.f;
		InitialLookRotation.Normalize();
		RuntimeData->CurrentLookRotation = InitialLookRotation;
	}

	// Interpolate to target rotation
	FRotator CurrentLookRotation = RuntimeData->CurrentLookRotation.GetValue();
	CurrentLookRotation.Roll = 0.f;
	CurrentLookRotation.Normalize();

	FRotator NewRotation = CurrentLookRotation;
	if (::IsValid(TypedSetting) && RuntimeData->TargetRotationInterpolationSpeedInfo.IsSet())
	{
		NewRotation = UOasisCameraFunctionLibrary::InterpolateRotator(
			CurrentLookRotation,
			TargetLookRotation,
			DeltaTime,
			RuntimeData->TargetRotationInterpolationSpeedInfo.GetValue()
		);
		NewRotation.Roll = 0.f;
		NewRotation.Normalize();
	}
	else
	{
		// No interpolation, set directly
		NewRotation = TargetLookRotation;
	}

	// Apply to view
	InOutView.Rotation = NewRotation;
	InOutView.ControlRotation = NewRotation;
	RuntimeData->CurrentLookRotation = NewRotation;

	// Draw debug
	if (::IsValid(TypedSetting) && TypedSetting->bDrawDebug)
	{
		DrawDebugPoint(InProxy->GetWorld(), TargetPoint, 20.0f, FColor::Green);
		DrawDebugLine(InProxy->GetWorld(), InOutView.Location, TargetPoint, FColor::Green, false, -1.f, 0, 1.0f);
	}
}
// From Penguin Assistant End
