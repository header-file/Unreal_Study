// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"

UCLASS()
class MYPROJECT_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* Explosion;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Damaged();

private:
	int HP;
};
