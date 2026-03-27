// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_FOV.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_FOV : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

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

