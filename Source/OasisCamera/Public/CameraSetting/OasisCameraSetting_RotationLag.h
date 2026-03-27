// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_RotationLag.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_RotationLag : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

	TOptional<FRotator> OldRotator;

};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_RotationLag : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetRotationLagSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_RotationLag::StaticClass();
	}

	/** Controls how quickly camera reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bUseCameraLagIteration = false;

	/** Max time step used when iterate camera lag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (editcondition = "bUseCameraLagSubstepping", ClampMin = "0.005", ClampMax = "0.5", UIMin = "0.005", UIMax = "0.5"))
	float CameraLagMaxTimeStep = 1 / 60.f;

};
