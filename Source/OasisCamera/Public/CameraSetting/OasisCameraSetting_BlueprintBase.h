// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once
#include "OasisCameraType.h"

#include "OasisCameraSetting_BlueprintBase.generated.h"

UCLASS(Blueprintable)
class UOasisCameraSettingRuntimeData_BlueprintBase : public UOasisCameraSettingRuntimeDataBase
{
	GENERATED_BODY()
public:

};

UCLASS()
class UOasisCameraSetting_BlueprintBase : public UOasisCameraSettingBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SettingTypeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOasisCameraSettingRuntimeData_BlueprintBase> RuntimeDataClass;

	virtual FName GetAssetTypeName() const override
	{
		return SettingTypeName;
	}

	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() override
	{
		return RuntimeDataClass;
	}
};
