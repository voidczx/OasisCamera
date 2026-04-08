// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OasisCameraType.h"

#include "OasisCameraProxy_LookAt.generated.h"

// From Penguin Assistant Start
UCLASS()
class UOasisCameraProxy_LookAt : public UOasisCameraProxyBase
{
	GENERATED_BODY()
public:

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void OnDynamicSettingChanged(const FName& SettingTypeName, const UOasisCameraSettingBase* PreviousSetting, const UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData) override;
	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) override;
};
// From Penguin Assistant End
