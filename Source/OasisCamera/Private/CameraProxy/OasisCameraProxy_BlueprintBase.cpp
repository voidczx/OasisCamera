// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_BlueprintBase.h"

void UOasisCameraProxy_BlueprintBase::Activate()
{
	ReceiveActivate();
}

void UOasisCameraProxy_BlueprintBase::Deactivate()
{
	ReceiveDeactivate();
}

// From Penguin Assistant Start
void UOasisCameraProxy_BlueprintBase::OnDynamicSettingChanged(const FName& SettingTypeName, const UOasisCameraSettingBase* PreviousSetting, const UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData)
{
	Super::OnDynamicSettingChanged(SettingTypeName, PreviousSetting, CurrentSetting, RuntimeData);
	ReceiveDynamicSettingChanged(
		SettingTypeName,
		const_cast<UOasisCameraSettingBase*>(PreviousSetting),
		const_cast<UOasisCameraSettingBase*>(CurrentSetting),
		RuntimeData
	);
}
// From Penguin Assistant End

void UOasisCameraProxy_BlueprintBase::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UpdateView_BlueprintImplement(DefaultCameraView, DeltaTime, InOutView);
}

