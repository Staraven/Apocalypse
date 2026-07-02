#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "RevolverWeapon.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API ARevolverWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARevolverWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Effects")
	class UNiagaraSystem* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Effects")
	FName MuzzleReferenceBoneName = "SK_Wep_Revolver_01";

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Effects")
	FVector MuzzleLocalOffset = FVector(20.0f, 0.0f, 5.0f);

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Effects")
	FRotator MuzzleEffectRotationOffset = FRotator::ZeroRotator;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PlayFireVisuals() override;
	virtual void PlayReloadVisuals() override;

	// Bone names can be adjusted per revolver variant in the editor.
	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Bones")
	FName HammerBoneName = "SK_Wep_Revolver_01_Hammer_01";

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Bones")
	FName CylinderBoneName = "SK_Wep_Revolver_01_Cylinder_Spin_01";

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Bones")
	FName TriggerBoneName = "SK_Wep_Revolver_01_Trigger_01";

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Bones")
	TArray<FName> CylinderBulletBoneNames = {
		"SK_Wep_Revolver_01_Cylinder_Bullet_01",
		"SK_Wep_Revolver_01_Cylinder_Bullet_02",
		"SK_Wep_Revolver_01_Cylinder_Bullet_03",
		"SK_Wep_Revolver_01_Cylinder_Bullet_04",
		"SK_Wep_Revolver_01_Cylinder_Bullet_05",
		"SK_Wep_Revolver_01_Cylinder_Bullet_06"
	};

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Animation")
	float FireAnimDuration = 0.18f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Animation")
	float ReloadAnimDuration = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Animation")
	float HammerKickbackAngle = -35.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Revolver|Animation")
	float CylinderStepAngle = 60.0f;

private:
	FTransform HammerRestTransform;
	FTransform TriggerRestTransform;
	FTransform CylinderRestTransform;
	TArray<FTransform> BulletRestTransforms;

	float ReloadAnimElapsed = 0.0f;
	float FireAnimElapsed = 0.0f;
	float CylinderRotationAtFireStart = 0.0f;
	float CurrentCylinderRotation = 0.0f;

	int32 NextBulletToHide = 0;
	int32 NextBulletToReveal = 0;

	void UpdateFireAnimation(float DeltaTime);
	bool CacheBoneRestTransform(FName BoneName, FTransform& OutTransform) const;
	void UpdateReloadAnimation(float DeltaTime);

	bool bIsAnimatingReload = false;
	bool bIsAnimatingFire = false;
};