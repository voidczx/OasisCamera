// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_CollisionTest.h"

void UOasisCameraProxy_CollisionTest::Activate()
{

}

void UOasisCameraProxy_CollisionTest::Deactivate()
{

}

void UOasisCameraProxy_CollisionTest::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_CollisionTest(this, DefaultCameraView, DeltaTime, InOutView);
}

