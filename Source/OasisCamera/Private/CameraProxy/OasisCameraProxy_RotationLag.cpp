// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_RotationLag.h"

// From Penguin Assistant Start
#include "CameraSetting/OasisCameraSetting_RotationLag.h"
// From Penguin Assistant End

void UOasisCameraProxy_RotationLag::Activate()
{

}

void UOasisCameraProxy_RotationLag::Deactivate()
{
	// From Penguin Assistant Start
	UOasisCameraSettingRuntimeData_RotationLag* RuntimeData = Cast<UOasisCameraSettingRuntimeData_RotationLag>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(this, UOasisCameraSettingTypeDictionary::GetRotationLagSettingTypeName())
	);
	if (::IsValid(RuntimeData))
	{
		RuntimeData->OldRotator.Reset();
	}
	// From Penguin Assistant End
}

void UOasisCameraProxy_RotationLag::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_RotationLag(this, DefaultCameraView, DeltaTime, InOutView);
}

