// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once
#include "OasisCameraType.h"

#include "OasisCameraSetting_BasicRotationLimit.generated.h"


UCLASS()
class UOasisCameraSettingRuntimeData_BasicRotationLimit : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()

public:

	// From Penguin Assistant Start
	virtual void ClearRuntimeInterpolationData() override
	{
		BasicRotationLimitInterpolationSpeedInfo.Reset();
	}

	UFUNCTION(BlueprintPure)
	bool TryGetBasicRotationLimitInterpolationSpeedInfo(FRuntimeOasisCameraInterpolationSpeedInfo& OutValue) const
	{
		if (BasicRotationLimitInterpolationSpeedInfo.IsSet())
		{
			OutValue = BasicRotationLimitInterpolationSpeedInfo.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetBasicRotationLimitInterpolationSpeedInfo(const FRuntimeOasisCameraInterpolationSpeedInfo& InValue)
	{
		BasicRotationLimitInterpolationSpeedInfo = InValue;
	}
	// From Penguin Assistant End

	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> BasicRotationLimitInterpolationSpeedInfo;

};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_BasicRotationLimit : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetBasicRotationLimitSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_BasicRotationLimit::StaticClass();
	}

	/** Minimum view pitch, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "-90.0", ClampMax = "0.0", UIMin = "-90.0", UIMax = "0.0"))
	float ViewPitchMin = -80.f;
	/** Maximum view pitch, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"))
	float ViewPitchMax = 80.f;
	/** Minimum view yaw, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "359.999", UIMin = "0.0", UIMax = "359.999"))
	float ViewYawMin = 0.f;
	/** Maximum view yaw, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "359.999", UIMin = "0.0", UIMax = "359.999"))
	float ViewYawMax = 359.9999f;
	/**
	* Rate of change of camera view(pitch and yaw)
	* Interpolation used for validation settings between different camera modes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FOasisCameraInterpolationSpeedInfo ViewInterpolation;

};

