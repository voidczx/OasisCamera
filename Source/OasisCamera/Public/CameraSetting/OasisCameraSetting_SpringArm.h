
// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once
#include "OasisCameraType.h"
#include "Curves/CurveVector.h"

#include "OasisCameraSetting_SpringArm.generated.h"

UCLASS()
class UOasisCameraSettingRuntimeData_SpringArm : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	bool TryGetFocusPoint(FVector& OutVector) const
	{
		if (TargetSpringArmStartPosition.IsSet())
		{
			OutVector = TargetSpringArmStartPosition.GetValue();
			return true;
		}
		return false;
	}

	TOptional<float> TargetDistance;
	TOptional<float> OldDistance;
	TOptional<FVector> OldSpringArmStartOffset;
	TOptional<FVector> OldSpringArmStartPosition;
	TOptional<FVector> TargetSpringArmStartPosition;
	TOptional<FVector> OldSpringArmEndOffset;
	TOptional<FVector> TargetSpringArmEndOffset;
	TOptional<FVector> OldSpringArmEndPosition;
	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> SpringArmStartOffsetInterpolationSpeedInfo;
	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> SpringArmEndOffsetInterpolationSpeedInfo;
	bool bPauseUseSpringArmEndOffsetByCurve = false;
};

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_SpringArm : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return FName("SpringArm");
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_SpringArm::StaticClass();
	}

	/** Minimal camera distance of spring arm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spring Arm Distance", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MinCameraDistance = 100.f;

	/** Maximum camera distance of spring arm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spring Arm Distance", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxCameraDistance = 1000.f;

	/** Default camera distance of spring arm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spring Arm Distance", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefaultCameraDistance = 500.0f;

	/**
	* If is not null, add camera distance depending on current pitch rotation
	* Value - additional distance
	* Time - normalize pitch rotation
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spring Arm Distance")
	TObjectPtr<UCurveFloat> PitchDistanceCurve = nullptr;

	/**
	* relative of camera rotation at end of spring arm;
	* X - x offset
	* Y - y offset
	* Z - z offset
	* Time - distance of spring arm
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Relative Socket Offset Curve", Category = "SpringArmEndOffset")
	TObjectPtr<UCurveVector> SpringArmEndRelativeOffsetCurve = nullptr;

	/** SocketOffset change speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Socket Offset Interpolation", Category = "SpringArmEndOffset")
	FOasisCameraInterpolationSpeedInfo SpringArmEndOffsetInterpolation;

	/** Offset at start of spring (host actor's location), applied in world space. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "World Target Offset", Category = "SpringArmStartOffset")
	FVector SpringArmStartWorldOffset = FVector::ZeroVector;

	/** TargetOffset change speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Target Offset Interpolation", Category = "SpringArmStartOffset")
	FOasisCameraInterpolationSpeedInfo SpringArmStartOffsetInterpolation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;

};



