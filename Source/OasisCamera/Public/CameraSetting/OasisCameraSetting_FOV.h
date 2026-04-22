// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_FOV.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_FOV : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

	// From Penguin Assistant Start
	virtual void ClearRuntimeInterpolationData() override
	{
		RuntimeFOVInterpolateInfo.Reset();
	}

	UFUNCTION(BlueprintPure)
	bool TryGetOldFOV(float& OutValue) const
	{
		if (OldFOV.IsSet())
		{
			OutValue = OldFOV.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetOldFOV(float InValue)
	{
		OldFOV = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetRuntimeFOVInterpolateInfo(FRuntimeOasisCameraInterpolationSpeedInfo& OutValue) const
	{
		if (RuntimeFOVInterpolateInfo.IsSet())
		{
			OutValue = RuntimeFOVInterpolateInfo.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetRuntimeFOVInterpolateInfo(const FRuntimeOasisCameraInterpolationSpeedInfo& InValue)
	{
		RuntimeFOVInterpolateInfo = InValue;
	}
	// From Penguin Assistant End

	TOptional<float> OldFOV;
	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> RuntimeFOVInterpolateInfo;

};

UCLASS()
class UOasisCameraSetting_FOV : public UOasisCameraSettingBase
{
	GENERATED_BODY()

public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetFOVSettingAssetTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_FOV::StaticClass();
	}

	/**
	 *	Final FOV will always add this value.
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BaseCameraFOV = 90.f;

	/**
	* Rate of change of field of view(FOV)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FOasisCameraInterpolationSpeedInfo FOVInterpolation;

	/*
	* Value - FOV modifier
	* Time - Distance until to character
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UCurveFloat> FOVByDistanceOfCharacterCurve = nullptr;

	/*
	* Value - FOV modifier
	* Time - Movement speed
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UCurveFloat> FOVByMovementSpeedCurve = nullptr;

};

