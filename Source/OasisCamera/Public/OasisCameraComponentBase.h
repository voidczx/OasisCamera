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

	UFUNCTION(BlueprintCallable)
	bool TryGetOldControlRotation(FRotator& OutRotator) const;

	UFUNCTION(BlueprintCallable)
	bool TryGetOldRotation(FRotator& OutRotator) const;

	UFUNCTION(BlueprintCallable)
	bool TryGetOldLocation(FVector& OutLocation) const;

	UFUNCTION(BlueprintCallable)
	bool TryGetOldFOV(float& OutFOV) const;

	UFUNCTION(BlueprintCallable)
	bool IsStackActive() const;

	UFUNCTION(BlueprintCallable)
	bool IsStackEmpty() const;

	UFUNCTION(BlueprintCallable)
	UOasisCameraModeBase* GetTopMode() const;

	UFUNCTION(BlueprintCallable)
	TArray<UOasisCameraModeBase*> GetAllActiveModes() const;

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
