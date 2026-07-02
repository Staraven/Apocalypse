// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#include "AI/ZombieCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieCharacter::ReceiveDamage_Implementation(float DamageAmount)
{
	if (IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	if (IsDead())
	{
		HandleDeath();
	}
}

void AZombieCharacter::AttackTarget(AActor* Target)
{
	if (!Target || IsDead()) return;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown)
	{
		return;
	}
	LastAttackTime = CurrentTime;

	float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (Distance <= AttackRange)
	{
		if (Target->Implements<UDamageable>())
		{
			IDamageable::Execute_ReceiveDamage(Target, AttackDamage);
		}

		UE_LOG(LogTemp, Warning, TEXT("ZombieCharacter: Attacked target %s."), *Target->GetName());
	}
}

bool AZombieCharacter::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

void AZombieCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("ZombieCharacter: Zombie died."));

	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);

	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		AICon->GetBrainComponent()->StopLogic("Zombie died");
	}

	UE_LOG(LogTemp, Warning, TEXT("ZombieCharacter: DeathMontage is %s."), DeathMontage ? TEXT("valid") : TEXT("null"));

	if (GetMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("ZombieCharacter: AnimInstance is %s."), GetMesh()->GetAnimInstance() ? TEXT("valid") : TEXT("null"));
	}

	if (DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		float MontageLength = GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
		UE_LOG(LogTemp, Warning, TEXT("ZombieCharacter: Montage_Play returned length: %f."), MontageLength);
	}
}