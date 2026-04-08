// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

#include "OasisCameraComponentBase.generated.h"

class UOasisCameraModeBase;
class UOasisCameraModeStack;
template <class TClass>
class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UOasisCameraModeBase>, FOasisCameraModeDelegate);

/**
 * UOasisCameraComponentBase
 *
 *	The base camera component class used by this project.
 */
UCLASS()
class OASISCAMERA_API UOasisCameraComponentBase : public UCameraComponent
{
	GENERATED_BODY()

public:
	UOasisCameraComponentBase(const FObjectInitializer &ObjectInitializer);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Oasis|Camera")
	static UOasisCameraComponentBase* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOasisCameraComponentBase>() : nullptr); }

	// Returns the target actor that the camera is looking at.
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Delegate used to query for the best camera mode.
	FOasisCameraModeDelegate DetermineCameraModeDelegate;

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

	UFUNCTION(BlueprintPure)
	bool TryGetOldControlRotation(FRotator& OutRotator) const;

	UFUNCTION(BlueprintPure)
	bool TryGetOldRotation(FRotator& OutRotator) const;

	UFUNCTION(BlueprintPure)
	bool TryGetOldLocation(FVector& OutLocation) const;

	UFUNCTION(BlueprintPure)
	bool TryGetOldFOV(float& OutFOV) const;

	UFUNCTION(BlueprintPure)
	bool IsStackActive() const;

	UFUNCTION(BlueprintPure)
	bool IsStackEmpty() const;

	UFUNCTION(BlueprintPure)
	UOasisCameraModeBase* GetTopMode() const;

	UFUNCTION(BlueprintPure)
	TArray<UOasisCameraModeBase*> GetAllActiveModes() const;

	// From Penguin Assistant Start
	// 允许外部更改 TopMode 的 ProxyState
	UFUNCTION(BlueprintCallable)
	void ChangeTopModeProxyState(const FName InStateName = NAME_None);

	// 允许外部获得 TopMode 指定 SettingName 的 SettingRuntimeData 指针
	UFUNCTION(BlueprintPure)
	class UOasisCameraSettingRuntimeDataBase* GetTopModeSettingRuntimeData(const FName SettingTypeName) const;

	// 允许外部为 TopMode 增加 DynamicSetting
	UFUNCTION(BlueprintCallable)
	void TryAddTopModeDynamicSetting(class UOasisCameraSettingBase* InDynamicSetting);

	// 允许外部从 TopMode 移除 DynamicSetting
	UFUNCTION(BlueprintCallable)
	void TryRemoveTopModeDynamicSetting(const FName SettingTypeName);
	// From Penguin Assistant End

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

protected:

	// when mode stack is active and empty, will push an instanced mode from this class.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OasisCameraComponent")
	TSubclassOf<UOasisCameraModeBase> DefaultModeClass;

	// Stack used to blend the camera modes.
	UPROPERTY()
	TObjectPtr<UOasisCameraModeStack> CameraModeStack;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset = 0.0f;

	TOptional<FRotator> OldRotation;
	TOptional<FRotator> OldControlRotation;
	TOptional<FVector> OldLocation;
	TOptional<float> OldFOV;
};
