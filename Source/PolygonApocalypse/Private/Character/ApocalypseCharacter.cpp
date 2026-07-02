// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#include "Character/ApocalypseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ZoneManager.h"
#include "Core/ApocalypsePlayerController.h"
#include "Interfaces/Damageable.h"
#include "GameFramework/SpringArmComponent.h"

AApocalypseCharacter::AApocalypseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), TEXT("head"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	GetMesh()->SetOwnerNoSee(true);
}

void AApocalypseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (AApocalypsePlayerController* ApocalypsePC = Cast<AApocalypsePlayerController>(PC))
		{
			ApocalypsePC->UpdateHealthBar(CurrentHealth, MaxHealth);
		}
	}

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(Camera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CurrentWeapon->SetActorRelativeLocation(FVector(30.0f, 15.0f, -20.0f));
			CurrentWeapon->SetOwner(this);
		}
	}
}

void AApocalypseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCameraRecoilRecovery(DeltaTime);
}

void AApocalypseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AApocalypseCharacter::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AApocalypseCharacter::Look);
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AApocalypseCharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AApocalypseCharacter::StopJumping);
		EIC->BindAction(TestDamageAction, ETriggerEvent::Started, this, &AApocalypseCharacter::TestDamage);
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &AApocalypseCharacter::Fire);
		EIC->BindAction(TestRestAction, ETriggerEvent::Started, this, &AApocalypseCharacter::TestRest);
		EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &AApocalypseCharacter::ReloadInput);
	}
}

void AApocalypseCharacter::TestRest(const FInputActionValue& Value)
{
	AZoneManager* ZoneManager = Cast<AZoneManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AZoneManager::StaticClass()));
	if (ZoneManager)
	{
		ZoneManager->OnPlayerRested();
	}
}

void AApocalypseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AApocalypseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AApocalypseCharacter::TestDamage(const FInputActionValue& Value)
{
	ApplyDamage(10.0f);
}

void AApocalypseCharacter::ApplyDamage(float DamageAmount)
{
	if (IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0, MaxHealth);

	if (AApocalypsePlayerController* PC = Cast<AApocalypsePlayerController>(GetController()))
	{
		PC->UpdateHealthBar(CurrentHealth, MaxHealth);
	}

	if (IsDead()) HandleDeath();
}

void AApocalypseCharacter::ApplyHeal(float HealAmount)
{
	if (IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (AApocalypsePlayerController* PC = Cast<AApocalypsePlayerController>(GetController()))
	{
		PC->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

bool AApocalypseCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

void AApocalypseCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("ApocalypseCharacter: Player died."));
}

void AApocalypseCharacter::Fire(const FInputActionValue& Value)
{
	if (CurrentWeapon && Camera)
	{
		FVector StartLocation = Camera->GetComponentLocation();
		FVector ForwardVector = Camera->GetForwardVector();

		bool bDidFire = CurrentWeapon->Fire(StartLocation, ForwardVector);

		if (bDidFire)
		{
			float Kick = CurrentWeapon->CameraRecoilPitch / 2.0f;
			CameraRecoilOffset += Kick;
			AddControllerPitchInput(-Kick);

			if (AApocalypsePlayerController* PC = Cast<AApocalypsePlayerController>(GetController()))
			{
				PC->TriggerCrosshairSpread(CurrentWeapon->CrosshairSpreadAmount);
			}
		}
	}
}

void AApocalypseCharacter::ReceiveDamage_Implementation(float DamageAmount)
{
	ApplyDamage(DamageAmount);
}

void AApocalypseCharacter::ReloadInput(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AApocalypseCharacter::UpdateCameraRecoilRecovery(float DeltaTime)
{
	if (FMath::IsNearlyZero(CameraRecoilOffset, 0.01f)) return;

	float DecayedValue = FMath::FInterpTo(CameraRecoilOffset, 0.0f, DeltaTime, CameraRecoilRecoverySpeed);
	float RecoveredThisFrame = CameraRecoilOffset - DecayedValue;
	CameraRecoilOffset = DecayedValue;

	AddControllerPitchInput(RecoveredThisFrame);
}