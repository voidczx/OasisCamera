// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OasisCameraType.h"

#include "OasisCameraProxy_BlueprintBase.generated.h"

UCLASS(Blueprintable)
class UOasisCameraProxy_BlueprintBase : public UOasisCameraProxyBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FName> DependencySettingAssetTypeNames;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) override;
	// From Penguin Assistant Start
	virtual void OnDynamicSettingChanged(const FName& SettingTypeName, const UOasisCameraSettingBase* PreviousSetting, const UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData) override;
	// From Penguin Assistant End

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDeactivate();

	// From Penguin Assistant Start
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDynamicSettingChanged(FName SettingTypeName, UOasisCameraSettingBase* PreviousSetting, UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData);
	// From Penguin Assistant End

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateView_BlueprintImplement(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);

};
