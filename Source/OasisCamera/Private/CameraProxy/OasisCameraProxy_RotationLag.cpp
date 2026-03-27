// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_RotationLag.h"

void UOasisCameraProxy_RotationLag::Activate()
{

}

void UOasisCameraProxy_RotationLag::Deactivate()
{

}

void UOasisCameraProxy_RotationLag::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_RotationLag(this, DefaultCameraView, DeltaTime, InOutView);
}

