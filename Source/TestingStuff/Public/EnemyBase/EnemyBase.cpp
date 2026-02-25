// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase/EnemyBase.h"



// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TargetingPointBox = CreateDefaultSubobject<UBoxComponent>(FName("TargetingPointBoxLocation"));
	TargetingPointBox->SetBoxExtent(FVector(5.f,5.f,5.f));
	TargetingPointBox->SetCollisionObjectType(ECollisionChannel::ECC_TARGETING);// Custom Object type ECC_TARGETING = ECC_GameTraceChannel1
	TargetingPointBox->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void AEnemyBase::ShowTarget_Implementation(bool bShow)
{
	// in blueprints
}

FTargetingData AEnemyBase::GetTargetingData_Implementation()
{
	FTargetingData Data;
	Data.bIsTargetable = bIsTargetable;
	Data.EnemyStateTag = StateTag;
	Data.EnemyTypeTag  = EnemyTypeTag;
	
	return Data;
}



FVector AEnemyBase::GetTargetingPointLocation_Implementation()
{
	// Return The location of the component Box collision
	return  TargetingPointBox->GetComponentLocation();
}

