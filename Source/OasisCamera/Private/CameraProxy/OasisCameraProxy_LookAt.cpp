// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

// From Penguin Assistant Start
#include "CameraProxy/OasisCameraProxy_LookAt.h"
#include "CameraSetting/OasisCameraSetting_LookAt.h"

void UOasisCameraProxy_LookAt::Activate()
{

}

void UOasisCameraProxy_LookAt::Deactivate()
{
	UOasisCameraSettingRuntimeData_LookAt* RuntimeData = Cast<UOasisCameraSettingRuntimeData_LookAt>(
		UOasisCameraFunctionLibrary::GetSettingRuntimeDataByProxy(this, UOasisCameraSettingTypeDictionary::GetLookAtSettingTypeName())
	);
	if (::IsValid(RuntimeData))
	{
		RuntimeData->TargetRotationInterpolationSpeedInfo.Reset();
		RuntimeData->CurrentLookRotation.Reset();
	}
}

void UOasisCameraProxy_LookAt::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	UOasisCameraFunctionLibrary::UpdateView_LookAt(this, DefaultCameraView, DeltaTime, InOutView);
}
// From Penguin Assistant End
