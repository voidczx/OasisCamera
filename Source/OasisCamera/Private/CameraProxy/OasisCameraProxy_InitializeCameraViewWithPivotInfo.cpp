// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_InitializeCameraViewWithPivotInfo.h"

#include "OasisCameraModeBase.h"

void UOasisCameraProxy_InitializeCameraViewWithPivotInfo::Activate()
{

}

void UOasisCameraProxy_InitializeCameraViewWithPivotInfo::Deactivate()
{

}

void UOasisCameraProxy_InitializeCameraViewWithPivotInfo::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	const UOasisCameraModeBase* HostMode = UOasisCameraFunctionLibrary::GetHostModeByProxy(this);
	if (::IsValid(HostMode))
	{
		InOutView.Location = HostMode->GetPivotLocation();
		InOutView.Rotation = HostMode->GetPivotRotation();
	}
	InOutView.ControlRotation = InOutView.Rotation;
}


