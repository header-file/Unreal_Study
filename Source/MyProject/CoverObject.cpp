// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverObject.h"

ACoverObject::ACoverObject()
{
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ACoverObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoverObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

