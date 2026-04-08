// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "Curves/CurveVector.h"
#include "GameplayTagContainer.h"
#include "OasisCameraType.h"

#include "OasisCameraModeBase.generated.h"

class AActor;
class UCanvas;
class UOasisCameraComponentBase;


/**
 * EOasisCameraModeBlendFunction
 *
 *	Blend function used for transitioning between camera modes.
 */
UENUM(BlueprintType)
enum class EOasisCameraModeBlendFunction : uint8
{
	// Does a simple linear interpolation.
	Linear,

	// Immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by the exponent.
	EaseIn,

	// Smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by the exponent.
	EaseOut,

	// Smoothly accelerates and decelerates.  Ease amount controlled by the exponent.
	EaseInOut,

	COUNT UMETA(Hidden)
};

// From Penguin Assistant Start
USTRUCT()
struct FOasisResolvedCameraSettingsSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TObjectPtr<UOasisCameraSettingBase> StaticSetting = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UOasisCameraSettingBase> DynamicSetting = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UOasisCameraSettingRuntimeDataBase> RuntimeData = nullptr;

	const UOasisCameraSettingBase* GetEffectiveSetting() const
	{
		return DynamicSetting ? DynamicSetting.Get() : StaticSetting.Get();
	}

	UOasisCameraSettingBase* GetMutableEffectiveSetting()
	{
		return DynamicSetting ? DynamicSetting.Get() : StaticSetting.Get();
	}

	bool HasAnySetting() const
	{
		return StaticSetting != nullptr || DynamicSetting != nullptr;
	}
};
// From Penguin Assistant End

/**
 * UOasisCameraModeBase
 *
 *	Base class for all camera modes.
 */
UCLASS(Abstract, Blueprintable)
class OASISCAMERA_API UOasisCameraModeBase : public UObject
{
	GENERATED_BODY()

public:
	UOasisCameraModeBase();

	UFUNCTION(BlueprintCallable)
	UOasisCameraComponentBase* GetOasisCameraComponent() const;

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

	const FOasisCameraModeView& GetCameraModeView() const { return View; }

	// Called when this camera mode is created on the camera mode stack.
	virtual void OnCreated();

	// Called when this camera mode is activated on the camera mode stack.
	virtual void OnActivation();

	// Called when this camera mode is deactivated on the camera mode stack.
	virtual void OnDeactivation();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveCreation();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveActivation();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDeactivation();

	UFUNCTION(BlueprintCallable)
	void ChangeProxyState(const FName& InStateName = NAME_None);
	UFUNCTION(BlueprintCallable)
	FName GetCurrentProxyState() const;

	UFUNCTION(BlueprintCallable)
	void TryCreateProxyByClass(TSubclassOf<UOasisCameraProxyBase> InClass);

	UFUNCTION(BlueprintCallable)
	void TryAddDynamicSetting(UOasisCameraSettingBase* InDynamicSetting);

	UFUNCTION(BlueprintCallable)
	void TryRemoveDynamicSetting(FName SettingTypeName);

	UFUNCTION(BlueprintCallable)
	UOasisCameraSettingRuntimeDataBase* GetSettingRuntimeData(const FName SettingTypeName) const;

	void UpdateCameraMode(const FMinimalViewInfo& DefaultCameraView, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float GetBlendTime() const { return BlendTime; }
	UFUNCTION(BlueprintCallable)
	float GetBlendWeight() const { return BlendWeight; }
	UFUNCTION(BlueprintCallable)
	void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	virtual void DrawDebug(UCanvas* Canvas) const;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetPivotLocation() const;
	UFUNCTION(BlueprintCallable)
	virtual FRotator GetPivotRotation() const;
	UFUNCTION(BlueprintCallable)
	const UOasisCameraSettingBase* GetReadOnlySetting(const FName& InSettingType) const;

	static FName DefaultProxyStateName;

protected:

	virtual void PreUpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime);
	virtual bool UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

protected:

	// A tag that can be queried by gameplay code that cares when a kind of camera mode is active
	// without having to ask about a specific mode (e.g., when aiming downsights to get more accuracy)
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	// View output produced by the camera mode.
	FOasisCameraModeView View;

	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// Function used for blending.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	EOasisCameraModeBlendFunction BlendFunction;

	// Exponent used by blend functions to control the shape of the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha;

	// Blend weight calculated using the blend alpha and function.
	float BlendWeight;

	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;

	//Whether this camera mode has a success update(UpdateView return true) during the activation
	UPROPERTY(transient)
	bool bSuccessUpdated = false;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OasisCameraMode")
	// TSet<UOasisCameraSettingBase*> ModeSettings;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "OasisCameraMode")
	TSet<UOasisCameraSettingBase*> InstancedModeSettings;

	UPROPERTY(EditDefaultsOnly, Category = "OasisCameraMode", DisplayName = "Default Proxy Classes")
	TObjectPtr<UOasisCameraProxyClassList> ProxyClasses;

	UPROPERTY(EditDefaultsOnly, Category = "OasisCameraMode", DisplayName = "Optional Proxy Classes")
	TMap<FName, TObjectPtr<UOasisCameraProxyClassList>> StateNameToProxyClasses;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UOasisCameraProxyBase>> ProxyInstances;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UOasisCameraProxyBase>> CachedProxyInstances;

	// From Penguin Assistant Start
	UPROPERTY(Transient)
	TMap<FName, FOasisResolvedCameraSettingsSlot> ResolvedSettingsByType;
	// From Penguin Assistant End

	UPROPERTY(Transient)
	bool bActive = false;

	UPROPERTY(Transient)
	FName CurrentProxyState = NAME_None;

private:

	// From Penguin Assistant Start
	void NotifyProxyDynamicSettingChanged(const FName& SettingTypeName, const UOasisCameraSettingBase* PreviousSetting, const UOasisCameraSettingBase* CurrentSetting, UOasisCameraSettingRuntimeDataBase* RuntimeData);
	void RefreshRuntimeDataForResolvedSetting(FOasisResolvedCameraSettingsSlot& InOutResolvedSetting);
	// From Penguin Assistant End

	void InternalOnCreate();
};

/**
 * UOasisCameraModeStack
 *
 *	Stack used for blending camera modes.
 */
UCLASS()
class OASISCAMERA_API UOasisCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UOasisCameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UOasisCameraModeBase> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FMinimalViewInfo& DefaultCameraView, FOasisCameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

	bool IsStackEmpty() const { return CameraModeStack.IsEmpty(); }

	TObjectPtr<UOasisCameraModeBase> GetTopMode() const;
	TArray<TObjectPtr<UOasisCameraModeBase>> GetAllActiveMode() const;

	// From Penguin Assistant Start
	// 允许外部更改 TopMode 的 ProxyState
	UFUNCTION(BlueprintCallable)
	void ChangeTopModeProxyState(const FName& InStateName = NAME_None);

	// 允许外部获得 TopMode 指定 SettingName 的 SettingRuntimeData 指针
	UFUNCTION(BlueprintCallable)
	class UOasisCameraSettingRuntimeDataBase* GetTopModeSettingRuntimeData(const FName SettingTypeName) const;
	// From Penguin Assistant End

protected:
	UFUNCTION()
	UOasisCameraModeBase* GetCameraModeInstance(TSubclassOf<UOasisCameraModeBase> CameraModeClass);

	void UpdateStack(FMinimalViewInfo& DefaultCameraView, float DeltaTime);
	void BlendStack(FOasisCameraModeView &OutCameraModeView) const;

protected:
	bool bIsActive = false;

	UPROPERTY()
	TArray<TObjectPtr<UOasisCameraModeBase>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UOasisCameraModeBase>> CameraModeStack;
};
