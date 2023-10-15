// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverObject.generated.h"

UCLASS()
class MYPROJECT_API ACoverObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoverObject();

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
