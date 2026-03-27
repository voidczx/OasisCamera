// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_BasicRotationLimit.h"

void UOasisCameraProxy_BasicRotationLimit::Activate()
{

}

void UOasisCameraProxy_BasicRotationLimit::Deactivate()
{

}

void UOasisCameraProxy_BasicRotationLimit::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_BasicRotationLimit(this, DefaultCameraView, DeltaTime, InOutView);
}

