// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyProjectCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return DefaultCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return DefaultCameraBoom; }

	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Camera)
		class UCameraComponent* DefaultCameraComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
		class USpringArmComponent* DefaultCameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera)
		class UCameraComponent* MoveCameraComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
		class USpringArmComponent* MoveCameraBoom;
	
	UPROPERTY(EditAnywhere, Category = Camera)
		class UCameraComponent* AimCameraComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
		class USpringArmComponent* AimCameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* ShowCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* ShowCameraBoom;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* MI_Highlight;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* MI_Default;

	UPROPERTY(EditAnyWhere)
		class UAnimMontage* MontShoot;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> FloorDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
		bool IsAiming;

	UPROPERTY(EditAnywhere)
		bool IsShooting;

	UPROPERTY(EditAnywhere)
		bool IsCrouching;

	UFUNCTION(BlueprintCallable)
		void ShowDefault();

	UFUNCTION(BlueprintCallable)
		void ShowMove();

	UFUNCTION(BlueprintCallable)
		void ShowAim();

	UFUNCTION(BlueprintCallable)
		void Show();

	UFUNCTION(BlueprintCallable)
		void Shoot();

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsAiming() { return IsAiming; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsShooting() { return IsShooting; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsCrouching() { return IsCrouching; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetIsShooting(bool b) { IsShooting = b; }

	FORCEINLINE void SetIsAiming(bool b) { IsAiming = b; }

	void Move(const FInputActionValue& value);
	void SpawnArea();
	void DisappearArea();

private:	
	int BulletCount;
	TArray<AActor*> Decals;
};

