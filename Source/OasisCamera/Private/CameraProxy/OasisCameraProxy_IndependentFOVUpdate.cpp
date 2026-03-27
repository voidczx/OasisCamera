#include "CameraProxy/OasisCameraProxy_IndependentFOVUpdate.h"


void UOasisCameraProxy_IndependentFOVUpdate::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_FOV(this, DefaultCameraView, DeltaTime, InOutView);
}
