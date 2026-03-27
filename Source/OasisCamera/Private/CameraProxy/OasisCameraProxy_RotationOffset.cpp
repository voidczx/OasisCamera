// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_RotationOffset.h"

void UOasisCameraProxy_RotationOffset::Activate()
{

}

void UOasisCameraProxy_RotationOffset::Deactivate()
{

}

void UOasisCameraProxy_RotationOffset::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	if (UOasisCameraFunctionLibrary::CanRoofCollisionChangeRotationOffset(this))
	{
		UOasisCameraFunctionLibrary::UpdateView_RoofCollisionChangeRotationOffset(this, DefaultCameraView, DeltaTime, InOutView);
	}
	else
	{
		UOasisCameraFunctionLibrary::UpdateView_BasicRotationOffset(this, DefaultCameraView, DeltaTime, InOutView);
	}
}


