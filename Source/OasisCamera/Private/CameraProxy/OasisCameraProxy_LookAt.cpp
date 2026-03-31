// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

// From Penguin Assistant Start
#include "CameraProxy/OasisCameraProxy_LookAt.h"

void UOasisCameraProxy_LookAt::Activate()
{

}

void UOasisCameraProxy_LookAt::Deactivate()
{

}

void UOasisCameraProxy_LookAt::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_LookAt(this, DefaultCameraView, DeltaTime, InOutView);
}
// From Penguin Assistant End
