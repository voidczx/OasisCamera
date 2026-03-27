// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "OasisCameraType.h"

#include "OasisCameraProxy_IndependentFOVUpdate.generated.h"

UCLASS()
class UOasisCameraProxy_IndependentFOVUpdate : public UOasisCameraProxyBase
{
	GENERATED_BODY()

public:

	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) override;

};

