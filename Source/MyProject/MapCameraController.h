// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyProjectCharacter.h"
#include "MapCameraController.generated.h"

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	CameraMode UMETA(DisplayName = "Camera"),	
	MoveMode UMETA(DisplayName = "Move"),	
	AttackMode UMETA(DisplayName = "Attack"),
};

UCLASS()
class MYPROJECT_API AMapCameraController : public APlayerController
{
	GENERATED_BODY()

public :
	AMapCameraController();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* DragAction;

	UPROPERTY(EditAnywhere, Category = SubClass)
		TSubclassOf<AActor> CoverObj;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enum)
		EPlayerMode PlayerMode;

	void FinishAttack();
	void CheckCoverObjAround();

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

	void Dragging(float DeltaSeconds);
	void CheckStopped();

	void OnMouseDown();
	void OnMouseDrag();
	void OnMouseUp();

	void CameraMove();
	void SelectPosition();
	void SelectEnemy();

	void PlayerMoveTo();

private:
	bool IsDragging;
	bool IsAttacking;
	FVector StartPos;
	AActor* SelectedActor;
	AActor* MapCamera;
	AMyProjectCharacter* Character;
};
