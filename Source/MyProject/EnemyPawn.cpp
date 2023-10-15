// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"
#include "Kismet/GameplayStatics.h"

AEnemyPawn::AEnemyPawn()
{
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HP = 5;
}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyPawn::Damaged()
{
	HP--;

	if (HP <= 0)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetTransform());

		this->ConditionalBeginDestroy();
	}
}

