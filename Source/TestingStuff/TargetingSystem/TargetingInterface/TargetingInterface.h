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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	bool IsTargetable();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	void ShowTarget(bool bShow);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	FGameplayTag GetStateTag();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Targeting")
	FVector GetTargetingPointLocation();
	
};


