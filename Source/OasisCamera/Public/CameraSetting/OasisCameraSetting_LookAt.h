// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_LookAt.generated.h"

// From Penguin Assistant Start
UCLASS()
class UOasisCameraSettingRuntimeData_LookAt : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()

public:

	virtual void ClearRuntimeInterpolationData() override
	{
		TargetRotationInterpolationSpeedInfo.Reset();
	}

	UFUNCTION(BlueprintPure)
	bool TryGetLockTargetPoint(FVector& OutVector) const
	{
		if (LockTargetPoint.IsSet())
		{
			OutVector = LockTargetPoint.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetLockTargetPoint(const FVector& InTargetPoint)
	{
		LockTargetPoint = InTargetPoint;
	}

	UFUNCTION(BlueprintPure)
	bool IsLockEnabled() const
	{
		return bIsLockEnabled;
	}

	UFUNCTION(BlueprintCallable)
	void SetLockEnabled(bool bInEnabled)
	{
		bIsLockEnabled = bInEnabled;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetTargetRotationInterpolationSpeedInfo(FRuntimeOasisCameraInterpolationSpeedInfo& OutValue) const
	{
		if (TargetRotationInterpolationSpeedInfo.IsSet())
		{
			OutValue = TargetRotationInterpolationSpeedInfo.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetTargetRotationInterpolationSpeedInfo(const FRuntimeOasisCameraInterpolationSpeedInfo& InValue)
	{
		TargetRotationInterpolationSpeedInfo = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetCurrentLookRotation(FRotator& OutValue) const
	{
		if (CurrentLookRotation.IsSet())
		{
			OutValue = CurrentLookRotation.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetCurrentLookRotation(const FRotator& InValue)
	{
		CurrentLookRotation = InValue;
	}

	TOptional<FVector> LockTargetPoint;
	bool bIsLockEnabled = true;
	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> TargetRotationInterpolationSpeedInfo;
	TOptional<FRotator> CurrentLookRotation;
};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_LookAt : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetLookAtSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_LookAt::StaticClass();
	}

	/** Whether the camera lock feature is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LookAt")
	bool bEnableLock = true;

	/** Default target point for camera to look at (used when no runtime override is set) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LookAt")
	FVector DefaultLockTargetPoint = FVector::ZeroVector;

	/** Rotation interpolation settings for smooth transition when lock target changes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LookAt")
	FOasisCameraInterpolationSpeedInfo RotationInterpolation;

	/** Draw debug information */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawDebug = false;
};
// From Penguin Assistant End
