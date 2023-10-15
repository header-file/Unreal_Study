// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "MapCameraController.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	DefaultCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	DefaultCameraBoom->SetupAttachment(RootComponent);
	DefaultCameraBoom->SetUsingAbsoluteRotation(true);
	DefaultCameraBoom->TargetArmLength = 800.f;
	DefaultCameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	DefaultCameraBoom->bDoCollisionTest = false;

	DefaultCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	DefaultCameraComponent->SetupAttachment(DefaultCameraBoom, USpringArmComponent::SocketName);
	DefaultCameraComponent->bUsePawnControlRotation = false;

	MoveCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MoveCameraBoom"));
	MoveCameraBoom->SetupAttachment(RootComponent);
	MoveCameraBoom->SetUsingAbsoluteRotation(true);
	MoveCameraBoom->TargetArmLength = 800.f;
	MoveCameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	MoveCameraBoom->bDoCollisionTest = false;

	MoveCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MoveCamera"));
	MoveCameraComponent->SetupAttachment(MoveCameraBoom, USpringArmComponent::SocketName);
	MoveCameraComponent->bUsePawnControlRotation = false;

	AimCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimCameraBoom"));
	AimCameraBoom->SetupAttachment(RootComponent);
	AimCameraBoom->SetUsingAbsoluteRotation(true);
	AimCameraBoom->TargetArmLength = 800.f;
	AimCameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	AimCameraBoom->bDoCollisionTest = false;
	
	AimCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCamera"));
	AimCameraComponent->SetupAttachment(AimCameraBoom, USpringArmComponent::SocketName);
	AimCameraComponent->bUsePawnControlRotation = false;

	ShowCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ShowCameraBoom"));
	ShowCameraBoom->SetupAttachment(RootComponent);
	ShowCameraBoom->SetUsingAbsoluteRotation(true);
	ShowCameraBoom->TargetArmLength = 400.f;
	ShowCameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	ShowCameraBoom->bDoCollisionTest = false;

	ShowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ShowCamera"));
	ShowCameraComponent->SetupAttachment(ShowCameraBoom, USpringArmComponent::SocketName);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	IsAiming = false;
	IsShooting = false;
	IsCrouching = false;
	BulletCount = 5;
}

void AMyProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMyProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
}

void AMyProjectCharacter::ShowDefault()
{
	DefaultCameraComponent->Activate();
	DefaultCameraComponent->SetActive(true);
	MoveCameraComponent->SetActive(false);
	AimCameraComponent->SetActive(false);
	ShowCameraComponent->SetActive(false);
}

void AMyProjectCharacter::ShowMove()
{
	MoveCameraComponent->Activate();
	DefaultCameraComponent->SetActive(false);
	MoveCameraComponent->SetActive(true);
	AimCameraComponent->SetActive(false);
	ShowCameraComponent->SetActive(false);
}

void AMyProjectCharacter::ShowAim()
{
	AimCameraComponent->Activate();
	DefaultCameraComponent->SetActive(false);
	MoveCameraComponent->SetActive(false);
	AimCameraComponent->SetActive(true);
	ShowCameraComponent->SetActive(false);
}

void AMyProjectCharacter::Show()
{
	ShowCameraComponent->Activate();
	DefaultCameraComponent->SetActive(false);
	MoveCameraComponent->SetActive(false);
	AimCameraComponent->SetActive(false);
	ShowCameraComponent->SetActive(true);
}

void AMyProjectCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MovementVector = value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMyProjectCharacter::SpawnArea()
{
	int index = 0;
	for(int i = -3; i < 4; i++)
		for (int j = -3; j < 4; j++)
		{
			FTransform trans;
			FVector pos = GetActorLocation();
			float x = 100.0f * i;
			float y = 100.0f * j;

			if (FMath::Abs(x) + FMath::Abs(y) >= 400.0f)
				continue;

			int iX = (int)(FMath::RoundHalfFromZero(pos.X / 100.0f)) * 100;
			int iY = (int)(FMath::RoundHalfFromZero(pos.Y / 100.0f)) * 100;

			pos.X = iX + x;
			pos.Y = iY + y;
			pos.Z = 0.0f;
			trans.SetLocation(pos);

			FActorSpawnParameters params;
			Decals.Add(GetWorld()->SpawnActor<AActor>(FloorDecal, trans, params));
		}
}

void AMyProjectCharacter::DisappearArea()
{
	for (int i = 0; i < Decals.Num(); i++)
		Decals[i]->ConditionalBeginDestroy();
}

void AMyProjectCharacter::Shoot()
{
	IsShooting = true;
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 100.0f, 1000.0f);

	BulletCount--;
	if (BulletCount <= 0)
	{
		ShowDefault();
		IsAiming = false;

		Cast<AMapCameraController>(GetController())->CheckCoverObjAround();
	}
}
