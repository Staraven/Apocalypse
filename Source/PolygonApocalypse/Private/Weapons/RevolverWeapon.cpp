#include "Weapons/RevolverWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/PoseableMeshComponent.h"

ARevolverWeapon::ARevolverWeapon()
{
}

bool ARevolverWeapon::CacheBoneRestTransform(FName BoneName, FTransform& OutTransform) const
{
	int32 BoneIndex = WeaponMesh->GetBoneIndex(BoneName);
	if (BoneIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("RevolverWeapon: Bone not found: %s."), *BoneName.ToString());
		return false;
	}

	// Passing identity as LocalToWorld returns the bone transform in component space.
	OutTransform = WeaponMesh->GetBoneTransform(BoneIndex, FTransform::Identity);
	return true;
}

void ARevolverWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponMesh) return;

	MaxAmmo = CylinderBulletBoneNames.Num();
	CurrentAmmo = MaxAmmo;

	CacheBoneRestTransform(HammerBoneName, HammerRestTransform);
	CacheBoneRestTransform(TriggerBoneName, TriggerRestTransform);
	CacheBoneRestTransform(CylinderBoneName, CylinderRestTransform);

	BulletRestTransforms.SetNum(CylinderBulletBoneNames.Num());
	for (int32 i = 0; i < CylinderBulletBoneNames.Num(); i++)
	{
		CacheBoneRestTransform(CylinderBulletBoneNames[i], BulletRestTransforms[i]);
	}
}

void ARevolverWeapon::PlayReloadVisuals()
{
	bIsAnimatingReload = true;
	ReloadAnimElapsed = 0.0f;
	NextBulletToReveal = 0;
}

void ARevolverWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAnimatingFire)
	{
		UpdateFireAnimation(DeltaTime);
	}

	if (bIsAnimatingReload)
	{
		UpdateReloadAnimation(DeltaTime);
	}
}

void ARevolverWeapon::PlayFireVisuals()
{
	if (!WeaponMesh) return;

	bIsAnimatingFire = true;
	FireAnimElapsed = 0.0f;
	CylinderRotationAtFireStart = CurrentCylinderRotation;
	CurrentCylinderRotation += CylinderStepAngle;

	if (CylinderBulletBoneNames.IsValidIndex(NextBulletToHide))
	{
		FTransform HiddenTransform = BulletRestTransforms[NextBulletToHide];
		HiddenTransform.SetScale3D(FVector::ZeroVector);
		WeaponMesh->SetBoneTransformByName(CylinderBulletBoneNames[NextBulletToHide], HiddenTransform, EBoneSpaces::ComponentSpace);
		NextBulletToHide++;
	}

	if (MuzzleFlashEffect)
	{
		int32 RefBoneIndex = WeaponMesh->GetBoneIndex(MuzzleReferenceBoneName);
		if (RefBoneIndex != INDEX_NONE)
		{
			FTransform RefBoneTransform = WeaponMesh->GetBoneTransform(RefBoneIndex);
			FVector MuzzleWorldLocation = RefBoneTransform.TransformPosition(MuzzleLocalOffset);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				MuzzleFlashEffect,
				MuzzleWorldLocation,
				RefBoneTransform.Rotator() + MuzzleEffectRotationOffset
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RevolverWeapon: Reference bone '%s' not found. Skipping muzzle flash."), *MuzzleReferenceBoneName.ToString());
		}
	}
}

void ARevolverWeapon::UpdateFireAnimation(float DeltaTime)
{
	FireAnimElapsed += DeltaTime;
	float Alpha = FMath::Clamp(FireAnimElapsed / FireAnimDuration, 0.0f, 1.0f);

	// Hammer pulls back during the first half, then snaps forward during the second half.
	float HammerAlpha = Alpha < 0.5f
		? Alpha / 0.5f
		: 1.0f - (Alpha - 0.5f) / 0.5f;

	FTransform HammerTM = HammerRestTransform;
	HammerTM.SetRotation(HammerRestTransform.GetRotation() * FRotator(0.0f, HammerKickbackAngle * HammerAlpha, 0.0f).Quaternion());
	WeaponMesh->SetBoneTransformByName(HammerBoneName, HammerTM, EBoneSpaces::ComponentSpace);

	FTransform TriggerTM = TriggerRestTransform;
	TriggerTM.SetRotation(TriggerRestTransform.GetRotation() * FRotator(0.0f, HammerAlpha * 15.0f, 0.0f).Quaternion());
	WeaponMesh->SetBoneTransformByName(TriggerBoneName, TriggerTM, EBoneSpaces::ComponentSpace);

	// Cylinder rotation persists instead of returning to the rest pose.
	float CurrentAngle = FMath::Lerp(CylinderRotationAtFireStart, CurrentCylinderRotation, Alpha);
	FTransform CylinderTM = CylinderRestTransform;
	CylinderTM.SetRotation(CylinderRestTransform.GetRotation() * FRotator(0.0f, 0.0f, CurrentAngle).Quaternion());
	WeaponMesh->SetBoneTransformByName(CylinderBoneName, CylinderTM, EBoneSpaces::ComponentSpace);

	if (Alpha >= 1.0f)
	{
		bIsAnimatingFire = false;
	}
}

void ARevolverWeapon::UpdateReloadAnimation(float DeltaTime)
{
	if (!WeaponMesh) return;

	ReloadAnimElapsed += DeltaTime;
	float Alpha = FMath::Clamp(ReloadAnimElapsed / ReloadAnimDuration, 0.0f, 1.0f);

	// Settle the cylinder first, then reveal the bullets near the end of the reload.
	float SettleAlpha = FMath::Clamp(Alpha / 0.7f, 0.0f, 1.0f);
	FTransform CylinderTM = CylinderRestTransform;
	CylinderTM.SetRotation(CylinderRestTransform.GetRotation() * FRotator(0.0f, 0.0f, CurrentCylinderRotation * (1.0f - SettleAlpha)).Quaternion());
	WeaponMesh->SetBoneTransformByName(CylinderBoneName, CylinderTM, EBoneSpaces::ComponentSpace);

	if (SettleAlpha >= 1.0f)
	{
		float RevealAlpha = FMath::Clamp((Alpha - 0.7f) / 0.3f, 0.0f, 1.0f);
		int32 BulletsToReveal = FMath::FloorToInt(RevealAlpha * CylinderBulletBoneNames.Num());
		for (int32 i = NextBulletToReveal; i < BulletsToReveal && i < CylinderBulletBoneNames.Num(); i++)
		{
			WeaponMesh->SetBoneTransformByName(CylinderBulletBoneNames[i], BulletRestTransforms[i], EBoneSpaces::ComponentSpace);
		}
		NextBulletToReveal = BulletsToReveal;
	}

	if (Alpha >= 1.0f)
	{
		// Ensure every bullet is visible at the end of the reload.
		for (int32 i = NextBulletToReveal; i < CylinderBulletBoneNames.Num(); i++)
		{
			WeaponMesh->SetBoneTransformByName(CylinderBulletBoneNames[i], BulletRestTransforms[i], EBoneSpaces::ComponentSpace);
		}

		bIsAnimatingReload = false;
		NextBulletToHide = 0;
		CurrentCylinderRotation = 0.0f;
		FinishReload();
	}
}