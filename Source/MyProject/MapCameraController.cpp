// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCameraController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CoverObject.h"

AMapCameraController::AMapCameraController()
{
	bShowMouseCursor = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	IsDragging = false;
	IsAttacking = false;
	MoveSpeed = 10.0f;

	PlayerMode = EPlayerMode::CameraMode;
}

void AMapCameraController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerMode == EPlayerMode::CameraMode && IsDragging)
		Dragging(DeltaSeconds);

	if (PlayerMode == EPlayerMode::MoveMode)
		CheckStopped();
}

void AMapCameraController::FinishAttack()
{
	
}

void AMapCameraController::CheckCoverObjAround()
{
	FVector pos = SelectedActor->GetActorLocation();
	pos.Z = 0.0f;

	TArray<AActor*> arrActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CoverObj, arrActors);
	for (int i = 0; i < arrActors.Num(); i++)
	{
		if (FVector::Dist(arrActors[i]->GetActorLocation(), pos) <= 140.0f)
			Character->IsCrouching = true;
	}
}

void AMapCameraController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	MapCamera = GetPawn();
}

void AMapCameraController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(DragAction, ETriggerEvent::Started, this, &AMapCameraController::OnMouseDown);
		EnhancedInputComponent->BindAction(DragAction, ETriggerEvent::Ongoing, this, &AMapCameraController::OnMouseDrag);
		EnhancedInputComponent->BindAction(DragAction, ETriggerEvent::Completed, this, &AMapCameraController::OnMouseUp);
		//EnhancedInputComponent->BindAction(DragAction, ETriggerEvent::Canceled, this, &AMapCameraController::OnMouseUp);
	}
}

void AMapCameraController::Dragging(float DeltaSeconds)
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		FVector curPos = Hit.Location;

		FVector pos = StartPos - curPos;
		GetPawn()->AddActorWorldOffset(FVector(pos.X, pos.Y, pos.Z) * MoveSpeed * DeltaSeconds);

		StartPos = curPos;
	}
}

void AMapCameraController::CheckStopped()
{
	FVector pos = SelectedActor->GetActorLocation();
	pos.Z = 0.0f;
	float dist = FVector::Dist(pos, StartPos);
	if (dist < 30.0f)
	{
		Character->ShowDefault();
		Character->SpawnArea();

		CheckCoverObjAround();

		PlayerMode = EPlayerMode::AttackMode;
	}
}

void AMapCameraController::OnMouseDown()
{
	if (SelectedActor != nullptr)
		SelectPosition();
	else
		CameraMove();

	if (PlayerMode == EPlayerMode::AttackMode)
		SelectEnemy();
}

void AMapCameraController::OnMouseDrag()
{
	
}

void AMapCameraController::OnMouseUp()
{
	IsDragging = false;

	if (SelectedActor != nullptr)
		PlayerMoveTo();
}

void AMapCameraController::CameraMove()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	if (bHitSuccessful)
	{
		if (Hit.GetActor()->GetClass()->IsChildOf<AMyProjectCharacter>())
		{
			SelectedActor = Hit.GetActor();
			Character = Cast<AMyProjectCharacter>(SelectedActor);
			

			if (GetPawn()->GetFName() == MapCamera->GetFName())
			{
				FVector targetPos = Hit.GetActor()->GetActorLocation();
				targetPos.Z = GetPawn()->GetActorLocation().Z;
				GetPawn()->SetActorLocation(targetPos);

				Character->SpawnArea();
			}

			this->Possess(Cast<APawn>(SelectedActor));
			Character->ShowDefault();
		}
		else
		{
			IsDragging = true;
			StartPos = Hit.Location;
		}
	}
}

void AMapCameraController::SelectPosition()
{
	if (PlayerMode == EPlayerMode::AttackMode)
		return;

	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		FVector aLoc = SelectedActor->GetActorLocation();
		aLoc.Z = 0.0f;
		if (FVector::Dist(Hit.Location, aLoc) >= 300.0f)
			return;

		float x = Hit.Location.X - SelectedActor->GetActorLocation().X;
		float y = Hit.Location.Y - SelectedActor->GetActorLocation().Y;
		int iX = (int)(FMath::RoundHalfFromZero(x / 100.0f)) * 100;
		int iY = (int)(FMath::RoundHalfFromZero(y / 100.0f)) * 100;
		StartPos.X = SelectedActor->GetActorLocation().X + iX;
		StartPos.Y = SelectedActor->GetActorLocation().Y + iY;
		StartPos.Z = 0.0f;

		PlayerMode = EPlayerMode::MoveMode;

		Character->DisappearArea();
	}
}

void AMapCameraController::SelectEnemy()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	if (bHitSuccessful)
	{
		if (Hit.GetActor()->GetClass()->IsChildOf<AEnemyPawn>())
		{
			Character->DisappearArea();
			Character->IsCrouching = false;

			if (Character->IsAiming && !Character->IsShooting)
			{
				Character->Shoot();
				Cast<AEnemyPawn>(Hit.GetActor())->Damaged();
				return;
			}

			if (!Character->IsAiming)
			{
				Character->ShowAim();

				FRotator rot = UKismetMathLibrary::FindLookAtRotation(SelectedActor->GetActorLocation(), Hit.GetActor()->GetActorLocation());
				SelectedActor->SetActorRotation(rot);
				Character->IsAiming = true;
			}
		}
	}
}

void AMapCameraController::PlayerMoveTo()
{
	if (PlayerMode == EPlayerMode::MoveMode)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, StartPos);
		Character->ShowMove();
	}
}
