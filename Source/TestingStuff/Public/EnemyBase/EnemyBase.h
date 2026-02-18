// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TestingStuff/TargetingSystem/TargetingInterface/TargetingInterface.h"
#include "EnemyBase.generated.h"

#define ECC_TARGETING ECC_GameTraceChannel1
UCLASS()
class TESTINGSTUFF_API AEnemyBase : public ACharacter, public ITargetingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Targeting Interface Functions
	virtual bool IsTargetable_Implementation() override;
	
	virtual void ShowTarget_Implementation(bool bShow) override;
	
	virtual FGameplayTag GetStateTag_Implementation() override;
	
	virtual FVector GetTargetingPointLocation_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	FGameplayTag StateTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	bool bIsTargetable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	FVector TargetingLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Box|Collision")
	TObjectPtr<UBoxComponent> TargetingPointBox;
};
