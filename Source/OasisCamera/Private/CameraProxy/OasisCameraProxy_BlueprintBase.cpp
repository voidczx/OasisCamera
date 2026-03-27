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

void UOasisCameraProxy_BlueprintBase::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UpdateView_BlueprintImplement(DefaultCameraView, DeltaTime, InOutView);
}

