// Copyright Beijing Bytedance Technology Co., Ltd. All Right Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OasisCameraType.generated.h"

/**
 * FOasisCameraModeView
 *
 *	View data produced by the camera mode that is used to blend camera modes.
 */
USTRUCT(BlueprintType)
struct OASISCAMERA_API FOasisCameraModeView
{

	GENERATED_USTRUCT_BODY()

public:

	void Blend(const FOasisCameraModeView &Other, float OtherWeight);
	void ApplyToMinimalView(FMinimalViewInfo &ViewInfo) const;
	void AssignedFromMinimalView(const FMinimalViewInfo &ViewInfo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector(ForceInit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator(ForceInit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ControlRotation = FRotator(ForceInit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FieldOfView = 80.0f;
};

//////////////////////////////////////////////////////////////////////////
// FOasisCameraModeView
//////////////////////////////////////////////////////////////////////////

inline void FOasisCameraModeView::Blend(const FOasisCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

inline void FOasisCameraModeView::ApplyToMinimalView(FMinimalViewInfo& ViewInfo) const
{
	ViewInfo.Location = Location;
	ViewInfo.Rotation = Rotation;
	ViewInfo.FOV = FieldOfView;
}

inline void FOasisCameraModeView::AssignedFromMinimalView(const FMinimalViewInfo& ViewInfo)
{
	Location = ViewInfo.Location;
	Rotation = ViewInfo.Rotation;
	FieldOfView = ViewInfo.FOV;
	ControlRotation = Rotation;
}

UCLASS(BlueprintType)
class UOasisCameraSettingRuntimeDataBase : public UObject
{
	GENERATED_BODY()
public:

};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class OASISCAMERA_API UOasisCameraSettingBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override final
	{
		return FPrimaryAssetId(GetAssetTypeName(), GetFName());
	}

	virtual FName GetAssetTypeName() const { return NAME_None; };
	virtual TSubclassOf<UOasisCameraSettingRuntimeDataBase> GetRuntimeDataClass() { return UOasisCameraSettingRuntimeDataBase::StaticClass(); }

};

UCLASS(Abstract, BlueprintType)
class OASISCAMERA_API UOasisCameraProxyBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void Activate() {}
	virtual void Deactivate() {}
	virtual void UpdateView(const FMinimalViewInfo& DefaultCameraView, float DeltaTime, FOasisCameraModeView& InOutView) {}

	UFUNCTION(BlueprintPure)
	virtual const UOasisCameraSettingBase* GetReadonlySetting(const FName SettingType) const;
	UFUNCTION(BlueprintPure)
	virtual UOasisCameraSettingRuntimeDataBase* GetSettingRuntimeData(const FName SettingType) const;

};

UCLASS(BlueprintType, Blueprintable)
class UOasisCameraProxyClassList : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UOasisCameraProxyBase>> ClassList;

};

UENUM(BlueprintType)
enum class EOasisCameraInterpolationType : uint8
{
	SmoothInterpolation, //Interpolation via  FMath::FInterpTo
	ConstInterpolation //Interpolation via FMath::FInterpConstantTo
};

USTRUCT(BlueprintType)
struct OASISCAMERA_API FOasisCameraInterpolationSpeedInfo
{
	GENERATED_BODY()
public:
	FOasisCameraInterpolationSpeedInfo() = default;
	explicit FOasisCameraInterpolationSpeedInfo(float InInterpolationSpeed)
		: InterpolationSpeed(InInterpolationSpeed)
	{
	}

public:
	/** Interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float InterpolationSpeed = 1.f;

	/** Interpolation type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	EOasisCameraInterpolationType InterpolationType = EOasisCameraInterpolationType::SmoothInterpolation;

	/** If true, @InterpolationSpeed will be interpolated with speed @InterpolationAcceleration and reset after interpolation result value is equals to interpolation target value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bAccelerateInterpolationSpeed = true;

	/** See @bAccelerateInterpolationSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "bUseInterpSpeedToInterpolation"))
	float InterpolationAcceleration = 1.f;

	/** Tolerance between result and target interpolation value for reset @InterpolationSpeed. See @bAccelerateInterpolationSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "bUseInterpSpeedToInterpolation"))
	float ResetInterpolationSpeedTolerance = 0.15f;
};

USTRUCT(BlueprintType)
struct FRuntimeOasisCameraInterpolationSpeedInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOasisCameraInterpolationSpeedInfo StaticInfo;

	UPROPERTY(BlueprintReadWrite)
	float CurrentInterpolationSpeed = 0.0f;

public:

	FRuntimeOasisCameraInterpolationSpeedInfo() = default;
	explicit FRuntimeOasisCameraInterpolationSpeedInfo(const FOasisCameraInterpolationSpeedInfo& InStaticInfo){ StaticInfo = InStaticInfo; ResetCurrentSpeed(); }

	void InterpolateSpeed(float DeltaTime);

	float GetCurrentInterpolationSpeed() const;

	void ResetCurrentSpeed();

	FString ToString() const;
};

inline void FRuntimeOasisCameraInterpolationSpeedInfo::InterpolateSpeed(float DeltaTime)
{
	if (StaticInfo.bAccelerateInterpolationSpeed)
	{
		CurrentInterpolationSpeed = FMath::FInterpConstantTo(CurrentInterpolationSpeed, StaticInfo.InterpolationSpeed, DeltaTime, StaticInfo.InterpolationAcceleration);
	}
}

inline float FRuntimeOasisCameraInterpolationSpeedInfo::GetCurrentInterpolationSpeed() const
{
	return StaticInfo.bAccelerateInterpolationSpeed ? CurrentInterpolationSpeed : StaticInfo.InterpolationSpeed;
}

inline void FRuntimeOasisCameraInterpolationSpeedInfo::ResetCurrentSpeed()
{
	if (StaticInfo.bAccelerateInterpolationSpeed)
	{
		CurrentInterpolationSpeed = 0.f;
	}
}

inline FString FRuntimeOasisCameraInterpolationSpeedInfo::ToString() const
{
	return FString::Printf(TEXT("%.2f"), GetCurrentInterpolationSpeed());
}

UCLASS()
class OASISCAMERA_API UOasisCameraSettingTypeDictionary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetBasicRotationOffsetSettingTypeName() { return FName("RotationOffset"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetRoofCollisionChangeRotationOffsetSettingTypeName() { return FName("RoofCollisionChangeRotationOffset"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetFallingChangeRotationOffsetSettingTypeName() { return FName("FallingChangeRotationOffset"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetFollowActorRotationSettingTypeName() { return FName("FollowActorRotation"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetBasicRotationLimitSettingTypeName() { return FName("BasicRotationLimit"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetRotationLagSettingTypeName() { return FName("RotationLag"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetSpringArmSettingTypeName() { return FName("SpringArm"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetLocationLagSettingTypeName() { return FName("LocationLag"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetCollisionTestSettingTypeName() { return FName("CollisionTest"); }
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetFOVSettingAssetTypeName() { return FName("FOV"); }
	// From Penguin Assistant Start
	UFUNCTION(BlueprintPure, Category = "OasisCameraSettingTypeDictionary")
	static FName GetLookAtSettingTypeName() { return FName("LookAt"); }
	// From Penguin Assistant End
};

UCLASS()
class OASISCAMERA_API UOasisCameraFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// interpolate
	//
	UFUNCTION(BlueprintCallable, Category = "OasisCamera|Utils")
	static FVector InterpolateVector(const FVector& Current, const FVector& Target, float DeltaTime, UPARAM(ref) FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget = true);
	UFUNCTION(BlueprintCallable, Category = "OasisCamera|Utils")
	static FRotator InterpolateRotator(const FRotator& Current, const FRotator& Target, float DeltaTime, UPARAM(ref) FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget = true);
	UFUNCTION(BlueprintCallable, Category = "OasisCamera|Utils")
	static float InterpolateFloat(const float& Current, const float& Target, float DeltaTime, UPARAM(ref) FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget = true);

	// component - mode - setting/proxy
	//
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static class UOasisCameraModeBase* GetHostModeByProxy(const UOasisCameraProxyBase* InProxy);
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static class UOasisCameraComponentBase* GetHostComponentByProxy(const UOasisCameraProxyBase* InProxy);
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static AActor* GetHostActorByProxy(const UOasisCameraProxyBase* InProxy);
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static APlayerCameraManager* GetHostCameraManagerByProxy(const UOasisCameraProxyBase* InProxy);
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static const UOasisCameraSettingBase* GetReadonlySettingByProxy(const UOasisCameraProxyBase* InProxy, const FName SettingTypeName);
	UFUNCTION(BlueprintPure, Category = "OasisCameraProxy|Utils")
	static UOasisCameraSettingRuntimeDataBase* GetSettingRuntimeDataByProxy(const UOasisCameraProxyBase* InProxy, const FName SettingTypeName);

	// for blueprint
	//
	UFUNCTION(BlueprintCallable, Category = "OasisCameraModeView|Utils")
	static void OasisCameraModeView_Blend(UPARAM(ref) FOasisCameraModeView& LHS, const FOasisCameraModeView& Other, float OtherWeight);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraModeView|Utils")
	static void OasisCameraModeView_ApplyToMinimalView(const FOasisCameraModeView& LHS, UPARAM(ref) FMinimalViewInfo &ViewInfo);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraModeView|Utils")
	static void OasisCameraModeView_AssignedFromMinimalView(UPARAM(ref) FOasisCameraModeView& LHS, const FMinimalViewInfo &ViewInfo);

	// camera view process with proxy library
	//
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_BasicRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_RoofCollisionChangeRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_FallingChangeRotationOffset(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_TryApplyRotationOffsetSetting(const UOasisCameraProxyBase* InProxy, const class UOasisCameraSetting_RotationOffset* InSetting, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_FollowActorRotation(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_BasicRotationLimit(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_RotationLag(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_SpringArm(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_LocationLag(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_CollisionTest(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	// From Penguin Assistant Start
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_LookAt(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);
	// From Penguin Assistant End
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static bool CanRoofCollisionChangeRotationOffset(const UOasisCameraProxyBase* InProxy);
	UFUNCTION(BlueprintCallable, Category = "OasisCameraImplement|CameraViewProcessWithProxy")
	static void UpdateView_FOV(const UOasisCameraProxyBase* InProxy, const FMinimalViewInfo& DefaultCameraView, float DeltaTime, UPARAM(ref) FOasisCameraModeView& InOutView);

private:

	// camera view process with proxy library internal function
	//
	UFUNCTION()
	static bool SphereCollisionTest(const UOasisCameraProxyBase* InProxy, FHitResult& OutResult, const class UOasisCameraSetting_CollisionTest* TypedSetting, const FVector& FocusPoint, const FVector& TraceEnd);
	UFUNCTION()
	static bool ConeCollisionTest(const UOasisCameraProxyBase* InProxy, FHitResult& InnerResult, FHitResult& OuterResult, const class UOasisCameraSetting_CollisionTest* TypedSetting, const FVector& FocusPoint, const FVector& TraceEnd);
	UFUNCTION()
	static void ProcessTargetCollisionCorrectionPosition(const UOasisCameraProxyBase* InProxy, const FHitResult& SphereHitResult, const FHitResult& ConeHitResult, const class UOasisCameraSetting_CollisionTest* TypedSetting, class UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, const FVector& LocationWithoutCorrection, const FVector& FocusPoint, const FVector& TraceEnd);
	UFUNCTION()
	static bool GetPositionAfterCollisionCorrection(const UOasisCameraProxyBase* InProxy, FVector& OutVector, const class UOasisCameraSetting_CollisionTest* TypedSetting, class UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, const FVector& CurrentLocation, const float DeltaTime);
	UFUNCTION()
	static bool GetPositionAfterCollisionCorrectionRecover(const UOasisCameraProxyBase* InProxy, const class UOasisCameraSetting_CollisionTest* TypedSetting, class UOasisCameraSettingRuntimeData_CollisionTest* TypedRuntimeData, FVector& OutVector, const FVector& FocusPoint, const FVector& PositionAfterCorrection, const FRotator& OldControlRotation, const FRotator& NewControlRotation, const float DeltaTime);
};

inline FVector UOasisCameraFunctionLibrary::InterpolateVector(const FVector& Current, const FVector& Target, float DeltaTime, FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget)
{
	InterpolationSpeedInfo.InterpolateSpeed(DeltaTime);
	FVector OutValue = Current;
	if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::SmoothInterpolation)
	{
		OutValue = FMath::VInterpTo(Current, Target, DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	else if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::ConstInterpolation)
	{
		OutValue = FMath::VInterpConstantTo(Current, Target, DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	if (bResetSpeedIfResultEqualToTarget && OutValue.Equals(Target, InterpolationSpeedInfo.StaticInfo.ResetInterpolationSpeedTolerance))
	{
		InterpolationSpeedInfo.ResetCurrentSpeed();
	}
	return OutValue;
}

inline FRotator UOasisCameraFunctionLibrary::InterpolateRotator(const FRotator& Current, const FRotator& Target, float DeltaTime, FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget)
{
	InterpolationSpeedInfo.InterpolateSpeed(DeltaTime);
	FQuat OutValue = Current.Quaternion();
	if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::SmoothInterpolation)
	{
		OutValue = FMath::QInterpTo(Current.Quaternion(), Target.Quaternion(), DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	else if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::ConstInterpolation)
	{
		OutValue = FMath::QInterpConstantTo(Current.Quaternion(), Target.Quaternion(), DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	if (bResetSpeedIfResultEqualToTarget && OutValue.Rotator().Equals(Target, InterpolationSpeedInfo.StaticInfo.ResetInterpolationSpeedTolerance))
	{
		InterpolationSpeedInfo.ResetCurrentSpeed();
	}
	return OutValue.Rotator();
}

inline float UOasisCameraFunctionLibrary::InterpolateFloat(const float& Current, const float& Target, float DeltaTime, FRuntimeOasisCameraInterpolationSpeedInfo& InterpolationSpeedInfo, bool bResetSpeedIfResultEqualToTarget)
{
	InterpolationSpeedInfo.InterpolateSpeed(DeltaTime);
	float OutValue = Current;
	if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::SmoothInterpolation)
	{
		OutValue = FMath::FInterpTo(Current, Target, DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	else if (InterpolationSpeedInfo.StaticInfo.InterpolationType == EOasisCameraInterpolationType::ConstInterpolation)
	{
		OutValue = FMath::FInterpConstantTo(Current, Target, DeltaTime, InterpolationSpeedInfo.GetCurrentInterpolationSpeed());
	}
	if (bResetSpeedIfResultEqualToTarget && FMath::IsNearlyEqual(OutValue, Target, InterpolationSpeedInfo.StaticInfo.ResetInterpolationSpeedTolerance))
	{
		InterpolationSpeedInfo.ResetCurrentSpeed();
	}
	return OutValue;
}

