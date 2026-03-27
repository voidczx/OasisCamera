// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_RoofCollisionChangeRotationOffset.generated.h"

UCLASS()
class OASISCAMERA_API UOasisCameraSetting_RoofCollisionChangeRotationOffset : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetRoofCollisionChangeRotationOffsetSettingTypeName();
	}

	/** List of collision objects to check(in trace)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes = { ObjectTypeQuery1 /*worldStatic*/ };

	/** Half size trace from actor eye(getting eyes viewpoint from AActor::GetActorEyesViewPoint using in BoxTrace) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector EyesBoxTraceHalfSize = FVector(450.f, 30, 5.f);

	/** Angle of rotation for character roof trace. See @ActorRoofBoxTraceHalfSize */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float RoofTraceAngleFirst = 15.f;

	/** Half size trace from character eyes(getting eyes viewpoint from AActor::GetActorEyesViewPoint using in BoxTrace) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector RoofBoxTraceHalfSizeFirst = FVector(600.f, 30.f, 90.f);

	/** Angle of rotation for camera roof trace. See @CameraRoofBoxTraceHalfSize */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float RoofTraceAngleSecond = 160.f;

	/** Half size trace from trace(using in BoxTrace) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector RoofBoxTraceHalfSizeSecond = FVector(100.f, 30.f, 60.f);

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UOasisCameraSetting_RotationOffset* RotationOffsetSettings;

};

