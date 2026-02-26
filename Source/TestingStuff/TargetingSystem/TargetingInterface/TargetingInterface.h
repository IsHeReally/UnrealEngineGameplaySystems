// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "TargetingInterface.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) struct FTargetingData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	bool bIsTargetable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	FGameplayTag EnemyStateTag; // What is the State
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	FGameplayTag EnemyTypeTag; // What is the Enemy Type
};
UINTERFACE(MinimalAPI, Blueprintable)
class  UTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

class TESTINGSTUFF_API ITargetingInterface 
{
	GENERATED_BODY()
	
public:
	// Functions
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interface|Targeting")
	FTargetingData GetTargetingData();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	void ShowTarget(const bool bShow);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	FVector GetTargetingPointLocation();
	
};


