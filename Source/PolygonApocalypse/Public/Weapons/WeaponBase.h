#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 5000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UPoseableMeshComponent* WeaponMesh;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool Fire(FVector StartLocation, FVector ForwardVector);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 MaxAmmo = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilKickback = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilPitch = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilRecoverySpeed = 12.0f;

	// Stored per weapon, applied by the character because the camera is owned by the character.
	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float CameraRecoilPitch = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float CrosshairSpreadAmount = 12.0f;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	UPROPERTY(EditAnywhere, Category = "Weapon|Audio")
	class USoundBase* FireSound;

protected:
	virtual void BeginPlay() override;

	// Weapon-specific visual hooks for subclasses.
	virtual void PlayFireVisuals();
	virtual void PlayReloadVisuals();

	bool bIsReloading = false;

	void FinishReload();
	void ApplyWeaponRecoilKick();
	void UpdateWeaponRecoilRecovery(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

private:
	float LastFireTime = 0.0f;
	bool bRecoilRestCached = false;

	FVector WeaponRestRelativeLocation;
	FRotator WeaponRestRelativeRotation;

	FVector RecoilOffsetLocation = FVector::ZeroVector;
	FRotator RecoilOffsetRotation = FRotator::ZeroRotator;
};