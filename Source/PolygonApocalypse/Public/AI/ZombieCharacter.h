#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AZombieCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AZombieCharacter();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DestroyDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 1.5f;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* DeathMontage;

	virtual void ReceiveDamage_Implementation(float DamageAmount) override;

protected:
	virtual void BeginPlay() override;

private:
	void HandleDeath();

	float LastAttackTime = 0.0f;
};