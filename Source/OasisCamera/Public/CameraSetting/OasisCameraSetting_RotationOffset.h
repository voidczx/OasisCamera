// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once
#include "OasisCameraType.h"

#include "OasisCameraSetting_RotationOffset.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_RotationOffset : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()

public:

	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> ApplyRotationOffsetInterpolationSpeedInfo;
	TOptional<float> CanApplyRotationOffsetAccumulateTime;

};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_RotationOffset : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetBasicRotationOffsetSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_RotationOffset::StaticClass();
	}

	/** Offset delay after the user manually changed rotation of camera (for example, moved the mouse or made an input via the gamepad)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartOffsetDelay = 1.2f;

	/** Pitch rotation change speed(from current to PitchOffset)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FOasisCameraInterpolationSpeedInfo PitchInterpolation;

	/** Pitch offset for camera relative 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float PitchOffset = -15.f;

	/** If true, offset settings will only be applied while Actor movement speed >= @MinMovementSpeedForActivate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bActivateOnlyIfMovement = true;

	/** Minimal actor movement speed for applied offset settings(if @bActivateOnlyIfMovement true)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bActivateOnlyIfMovement", EditConditionHides = true))
	float MinMovementSpeedForActivate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bPrintPitch = false;
};

