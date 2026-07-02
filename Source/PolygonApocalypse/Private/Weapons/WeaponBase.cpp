#include "Weapons/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Damageable.h"
#include "DrawDebugHelpers.h"
#include "Components/PoseableMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWeaponRecoilRecovery(DeltaTime);
}

void AWeaponBase::ApplyWeaponRecoilKick()
{
	// Cache the rest transform on first fire because the character sets the final relative transform after spawning the weapon.
	if (!bRecoilRestCached)
	{
		WeaponRestRelativeLocation = GetRootComponent()->GetRelativeLocation();
		WeaponRestRelativeRotation = GetRootComponent()->GetRelativeRotation();
		bRecoilRestCached = true;
	}

	RecoilOffsetLocation.X -= RecoilKickback;
	RecoilOffsetRotation.Pitch += RecoilPitch;
}

void AWeaponBase::UpdateWeaponRecoilRecovery(float DeltaTime)
{
	if (!bRecoilRestCached) return;
	if (RecoilOffsetLocation.IsNearlyZero(0.01f) && RecoilOffsetRotation.IsNearlyZero(0.01f)) return;

	RecoilOffsetLocation = FMath::VInterpTo(RecoilOffsetLocation, FVector::ZeroVector, DeltaTime, RecoilRecoverySpeed);
	RecoilOffsetRotation = FMath::RInterpTo(RecoilOffsetRotation, FRotator::ZeroRotator, DeltaTime, RecoilRecoverySpeed);

	GetRootComponent()->SetRelativeLocation(WeaponRestRelativeLocation + RecoilOffsetLocation);
	GetRootComponent()->SetRelativeRotation(WeaponRestRelativeRotation + RecoilOffsetRotation);
}

void AWeaponBase::PlayFireVisuals()
{
}

bool AWeaponBase::Fire(FVector StartLocation, FVector ForwardVector)
{
	if (bIsReloading) return false;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireRate)
	{
		return false;
	}

	if (MaxAmmo >= 0 && CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponBase: Out of ammo. Reload required."));
		return false;
	}

	LastFireTime = CurrentTime;

	FVector EndLocation = StartLocation + (ForwardVector * Range);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, StartLocation, EndLocation, ECC_Pawn, QueryParams
	);

	DrawDebugLine(GetWorld(), StartLocation, bHit ? HitResult.Location : EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->Implements<UDamageable>())
		{
			IDamageable::Execute_ReceiveDamage(HitActor, Damage);
		}

		UE_LOG(LogTemp, Warning, TEXT("WeaponBase: Hit %s."), HitActor ? *HitActor->GetName() : TEXT("Unknown"));
	}

	if (MaxAmmo >= 0)
	{
		CurrentAmmo--;
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, StartLocation);
	}

	PlayFireVisuals();
	ApplyWeaponRecoilKick();

	return true;
}

void AWeaponBase::Reload()
{
	if (bIsReloading) return;

	bIsReloading = true;
	PlayReloadVisuals();
}

void AWeaponBase::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}

void AWeaponBase::PlayReloadVisuals()
{
}