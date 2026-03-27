#include "CameraProxy/OasisCameraProxy_SpringArm.h"

void UOasisCameraProxy_SpringArm::Activate()
{

}

void UOasisCameraProxy_SpringArm::Deactivate()
{

}

void UOasisCameraProxy_SpringArm::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_SpringArm(this, DefaultCameraView, DeltaTime, InOutView);
}

