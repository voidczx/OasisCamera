// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#include "CameraProxy/OasisCameraProxy_ThirdPersonBasicTransform.h"

#include "OasisCameraComponentBase.h"
#include "OasisCameraModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UOasisCameraProxy_ThirdPersonBasicTransform::Activate()
{

}

void UOasisCameraProxy_ThirdPersonBasicTransform::Deactivate()
{

}

void UOasisCameraProxy_ThirdPersonBasicTransform::UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView)
{
	const UOasisCameraModeBase* HostMode = UOasisCameraFunctionLibrary::GetHostModeByProxy(this);
	if (::IsValid(HostMode))
	{
		InOutView.Location = HostMode->GetPivotLocation();
		InOutView.Rotation = HostMode->GetPivotRotation();
	}
	InOutView.ControlRotation = InOutView.Rotation;

	UOasisCameraFunctionLibrary::UpdateView_BasicRotationLimit(this, DefaultCameraView, DeltaTime, InOutView);
	UOasisCameraFunctionLibrary::UpdateView_RotationLag(this, DefaultCameraView, DeltaTime, InOutView);

	if (UOasisCameraFunctionLibrary::CanRoofCollisionChangeRotationOffset(this))
	{
		UOasisCameraFunctionLibrary::UpdateView_RoofCollisionChangeRotationOffset(this, DefaultCameraView, DeltaTime, InOutView);
	}
	else
	{
		UOasisCameraFunctionLibrary::UpdateView_BasicRotationOffset(this, DefaultCameraView, DeltaTime, InOutView);
	}
	UOasisCameraFunctionLibrary::UpdateView_FollowActorRotation(this, DefaultCameraView, DeltaTime, InOutView);

	UOasisCameraFunctionLibrary::UpdateView_SpringArm(this, DefaultCameraView, DeltaTime, InOutView);

	UOasisCameraFunctionLibrary::UpdateView_CollisionTest(this, DefaultCameraView, DeltaTime, InOutView);
}

