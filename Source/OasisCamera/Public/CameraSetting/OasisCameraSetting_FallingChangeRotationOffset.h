// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraSetting_FallingChangeRotationOffset.generated.h"

UCLASS()
class UOasisCameraSetting_FallingChangeRotationOffset : public UOasisCameraSettingBase
{
	GENERATED_BODY()

public:

	virtual FName GetAssetTypeName() const override
	{
		return UOasisCameraSettingTypeDictionary::GetFallingChangeRotationOffsetSettingTypeName();
	}

	/** List of collision objects to check(in trace)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes = { ObjectTypeQuery1 /*worldStatic*/ };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float TraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UOasisCameraSetting_RotationOffset* RotationOffsetSettings;

};
