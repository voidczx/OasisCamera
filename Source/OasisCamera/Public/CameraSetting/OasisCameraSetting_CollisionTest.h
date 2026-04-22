// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once
#include "OasisCameraType.h"

#include "OasisCameraSetting_CollisionTest.generated.h"


UCLASS()
class UOasisCameraSettingRuntimeData_CollisionTest : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

	// From Penguin Assistant Start
	virtual void ClearRuntimeInterpolationData() override
	{
		CollisionCorrectionInterpolationSpeedInfo.Reset();
		RecoverCollisionAffectedDistanceInterpolationSpeedInfo.Reset();
	}

	UFUNCTION(BlueprintPure)
	bool TryGetCollisionCorrectionInterpolationSpeedInfo(FRuntimeOasisCameraInterpolationSpeedInfo& OutValue) const
	{
		if (CollisionCorrectionInterpolationSpeedInfo.IsSet())
		{
			OutValue = CollisionCorrectionInterpolationSpeedInfo.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetCollisionCorrectionInterpolationSpeedInfo(const FRuntimeOasisCameraInterpolationSpeedInfo& InValue)
	{
		CollisionCorrectionInterpolationSpeedInfo = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetRecoverCollisionAffectedDistanceInterpolationSpeedInfo(FRuntimeOasisCameraInterpolationSpeedInfo& OutValue) const
	{
		if (RecoverCollisionAffectedDistanceInterpolationSpeedInfo.IsSet())
		{
			OutValue = RecoverCollisionAffectedDistanceInterpolationSpeedInfo.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetRecoverCollisionAffectedDistanceInterpolationSpeedInfo(const FRuntimeOasisCameraInterpolationSpeedInfo& InValue)
	{
		RecoverCollisionAffectedDistanceInterpolationSpeedInfo = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetCanRecoverCollisionAffectedDistanceAccumulateTime(float& OutValue) const
	{
		if (CanRecoverCollisionAffectedDistanceAccumulateTime.IsSet())
		{
			OutValue = CanRecoverCollisionAffectedDistanceAccumulateTime.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetCanRecoverCollisionAffectedDistanceAccumulateTime(float InValue)
	{
		CanRecoverCollisionAffectedDistanceAccumulateTime = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetOldCollisionCorrectionPosition(FVector& OutValue) const
	{
		if (OldCollisionCorrectionPosition.IsSet())
		{
			OutValue = OldCollisionCorrectionPosition.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetOldCollisionCorrectionPosition(const FVector& InValue)
	{
		OldCollisionCorrectionPosition = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetTargetCollisionCorrectionPosition(FVector& OutValue) const
	{
		if (TargetCollisionCorrectionPosition.IsSet())
		{
			OutValue = TargetCollisionCorrectionPosition.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetTargetCollisionCorrectionPosition(const FVector& InValue)
	{
		TargetCollisionCorrectionPosition = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetRelativeTargetCollisionCorrectionPosition(FVector& OutValue) const
	{
		if (RelativeTargetCollisionCorrectionPosition.IsSet())
		{
			OutValue = RelativeTargetCollisionCorrectionPosition.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetRelativeTargetCollisionCorrectionPosition(const FVector& InValue)
	{
		RelativeTargetCollisionCorrectionPosition = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetRelativeOldCollisionCorrectionPosition(FVector& OutValue) const
	{
		if (RelativeOldCollisionCorrectionPosition.IsSet())
		{
			OutValue = RelativeOldCollisionCorrectionPosition.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetRelativeOldCollisionCorrectionPosition(const FVector& InValue)
	{
		RelativeOldCollisionCorrectionPosition = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetCollisionAffectedDistance(float& OutValue) const
	{
		if (CollisionAffectedDistance.IsSet())
		{
			OutValue = CollisionAffectedDistance.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetCollisionAffectedDistance(float InValue)
	{
		CollisionAffectedDistance = InValue;
	}

	UFUNCTION(BlueprintPure)
	bool TryGetOldCollisionTraceDistance(float& OutValue) const
	{
		if (OldCollisionTraceDistance.IsSet())
		{
			OutValue = OldCollisionTraceDistance.GetValue();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetOldCollisionTraceDistance(float InValue)
	{
		OldCollisionTraceDistance = InValue;
	}
	// From Penguin Assistant End

	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> CollisionCorrectionInterpolationSpeedInfo;
	TOptional<FRuntimeOasisCameraInterpolationSpeedInfo> RecoverCollisionAffectedDistanceInterpolationSpeedInfo;
	TOptional<float> CanRecoverCollisionAffectedDistanceAccumulateTime;
	TOptional<FVector> OldCollisionCorrectionPosition;
	TOptional<FVector> TargetCollisionCorrectionPosition;
	TOptional<FVector> RelativeTargetCollisionCorrectionPosition;
	TOptional<FVector> RelativeOldCollisionCorrectionPosition;
	TOptional<float> CollisionAffectedDistance;
	TOptional<float> OldCollisionTraceDistance;
};

UCLASS()
class UOasisCameraSetting_CollisionTest : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetCollisionTestSettingTypeName();
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return UOasisCameraSettingRuntimeData_CollisionTest::StaticClass();
	}

	/** Collision channel of the query probe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|NormalCollisionProbe")
	TEnumAsByte<ECollisionChannel> ProbeChannel = ECC_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|NormalCollisionProbe")
	float SphereRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ConeCollisionProbe")
	bool bIgnoreConeCollisionWhenMove = false;

	// a collision correction will happen when a step angle's  inner layer and outer layer block something together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ConeCollisionProbe")
	float InnerLayerAngle = 5.0f;

	// a collision correction will happen when a step angle's  inner layer and outer layer block something together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ConeCollisionProbe")
	float OuterLayerAngle = 10.0f;

	// a collision correction will happen when a step angle's  inner layer and outer layer block something together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ConeCollisionProbe")
	float ConeStepAngle = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ConeCollisionProbe")
	FOasisCameraInterpolationSpeedInfo CollisionCorrectionInterpolationSpeedInfo;

	// there will be a (MaxDistance + ThisConfig)'s sphere trace, if hit, collision affected distance only be smaller when controller rotation change will apply.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|CollisionAffectedDistanceRecover")
	float BlockCollisionAffectedDistanceRecoverWhenControllerRotationChangedTraceDistanceOffset = -200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|CollisionAffectedDistanceRecover")
	float CollisionAffectedDistanceRecoverDelayTime = 0.5f;

	// if distance between two collision correction smaller than this value, there will be no defer recover.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|CollisionAffectedDistanceRecover")
	float DistanceOffsetOfImmediatelyApplyNewCorrectedLocation = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|CollisionAffectedDistanceRecover")
	FOasisCameraInterpolationSpeedInfo CollisionAffectedDistanceRecoverInterpolationSpeedInfo;

	/**
	* Ignoring collisions between the camera and the character(but not the camera itself) while camera moving for special objects. Enabled if standard collision test detected collision
	* see params @MovementCollisionTestDuration, @MovementCollisionTestMinLocationDelta, @MovementCollisionTestCollisionChannel
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|MovementCollisionTest")
	bool bDoMovementCollisionTest = true;

	/**
	* Duration of movement collision test after stopped movement
	* see @bDoMovementCollisionTest
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|MovementCollisionTest", meta = (Editcondition = "bDoMovementCollisionTest"))
	float MovementCollisionTestDuration = 1.f;

	/**
	* Minimal location delta for enable movement collision.
	* see @bDoMovementCollisionTest
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|MovementCollisionTest", meta = (Editcondition = "bDoMovementCollisionTest"))
	float MovementCollisionTestMinLocationDelta = 1.f;

	/**
	* Ignoring collision works only for those components has this tag.
	* see @bDoMovementCollisionTest
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|MovementCollisionTest", meta = (Editcondition = "bDoMovementCollisionTest"))
	FName MovementCollisionTestCollisionTag = NAME_None;

	// should overlap to any collision channel that has movement collision test tag.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|MovementCollisionTest", meta = (Editcondition = "bDoMovementCollisionTest"))
	TEnumAsByte<ECollisionChannel> ProbeChannelWhenMoving = ECC_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;
};


