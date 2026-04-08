// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OasisCameraType.h"

#include "OasisCameraProxy_RotationLag.generated.h"

UCLASS()
class UOasisCameraProxy_RotationLag : public UOasisCameraProxyBase
{
	GENERATED_BODY()
public:

	virtual void Activate() override;
	virtual void Deactivate() override;
	// From Penguin Assistant Start
	virtual void OnDynamicSettingChanged(const FName& SettingTypeName, const UOasisCameraSettingBase* PreviousSetting, const UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData) override;
	// From Penguin Assistant End
	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) override;

};
