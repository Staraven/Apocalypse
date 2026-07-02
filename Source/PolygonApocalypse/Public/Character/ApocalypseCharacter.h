// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/WeaponBase.h"
#include "Interfaces/Damageable.h"
#include "ApocalypseCharacter.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AApocalypseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	virtual void ReceiveDamage_Implementation(float DamageAmount) override;

public:
	AApocalypseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyHeal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* TestDamageAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY()
	class AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* TestRestAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ReloadAction;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void TestDamage(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void TestRest(const FInputActionValue& Value);
	void ReloadInput(const FInputActionValue& Value);

private:
	void HandleDeath();
	void UpdateCameraRecoilRecovery(float DeltaTime);

	float CameraRecoilOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float CameraRecoilRecoverySpeed = 10.0f;
};