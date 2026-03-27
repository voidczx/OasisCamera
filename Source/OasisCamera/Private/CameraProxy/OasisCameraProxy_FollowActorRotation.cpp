// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_FollowActorRotation.h"

void UOasisCameraProxy_FollowActorRotation::Activate()
{

}

void UOasisCameraProxy_FollowActorRotation::Deactivate()
{

}

void UOasisCameraProxy_FollowActorRotation::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_FollowActorRotation(this, DefaultCameraView, DeltaTime, InOutView);
}


