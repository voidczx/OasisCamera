// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_FollowActorRotation.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_FollowActorRotation : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()

public:

	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> FollowActorRotationInterpolationSpeedInfo;
	TOptional<float> CanFollowActorRotationAccumulateTime;

};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_FollowActorRotation : public UOasisCameraSettingBase
{
	GENERATED_BODY()

public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetFollowActorRotationSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_FollowActorRotation::StaticClass();
	}

	/** The maximum difference between yaw rotation of actor and camera to start following rotation of camera to rotation of actor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MaxDeltaForChange = 170.f;

	/** Delay after the user manually changed rotation of camera (for example, moved the mouse or made an input via the gamepad)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float StartChangeDelay = 0.7f;

	/** Change speed of rotation of camera in direction of rotation of actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FOasisCameraInterpolationSpeedInfo RotationInterpolation;

};

