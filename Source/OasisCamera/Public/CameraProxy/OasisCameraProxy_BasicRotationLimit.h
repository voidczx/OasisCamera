// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OasisCameraType.h"

#include "OasisCameraProxy_BasicRotationLimit.generated.h"

UCLASS()
class UOasisCameraProxy_BasicRotationLimit : public UOasisCameraProxyBase
{
	GENERATED_BODY()
public:

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) override;

};

