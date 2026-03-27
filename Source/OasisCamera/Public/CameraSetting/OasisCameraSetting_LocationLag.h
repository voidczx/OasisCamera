// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_LocationLag.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_LocationLag : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()

public:

	TOptional<FVector> OldLocation;
	TOptional<FVector> TargetLocation;

};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_LocationLag : public UOasisCameraSettingBase
{
	GENERATED_BODY()

public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetLocationLagSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_LocationLag::StaticClass();
	}

	/**
		* Controls how quickly camera reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraLagSpeedXY = 5.0f;
	/**
		* Controls how quickly camera reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraLagSpeedZ = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float CameraLagMaxDistanceXY = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float CameraLagMaxDistanceZ = 30.f;

	/**
	 * If true and camera location lag is enabled, draws markers at the camera target (in green) and the lagged position (in yellow).
	 * A line is drawn between the two locations, in green normally but in red if the distance to the lag target has been clamped (by CameraLagMaxDistance).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bDrawDebugLagMarkers = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bUseCameraLagIteration = false;
	/** Max time step used when iterate camera lag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (Editcondition = "bUseCameraLagSubstepping", ClampMin = "0.005", ClampMax = "0.5", UIMin = "0.005", UIMax = "0.5"))
	float CameraLagMaxTimeStep = 1 / 60.f;

};


