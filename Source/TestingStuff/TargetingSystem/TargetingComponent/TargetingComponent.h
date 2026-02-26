// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TargetingComponent.generated.h"

// Enum to Reserve or Reset Arrays in Blueprints
UENUM(BlueprintType)
enum class ETargetingPoolArraysType : uint8
{
	CyclingActors = 0  UMETA(DisplayName = "Main Actor" ),
	ActorsInRange  UMETA(DisplayName = "Actors In Range"),
	ActorsInSight  UMETA(DisplayName = "Actors In Sight"),
};


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class TESTINGSTUFF_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Actors|Main")
	TObjectPtr<AActor> TargetActor;
	// Tags for sorting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category="Targeting|Sorting|Settings|Tags")
	FGameplayTagContainer StateTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Targeting|Sorting|Settings|Tags")
	FGameplayTagContainer EnemyTypeTags;
	
	// Range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Range")
	float TargetingRange; // if differnt classes -> different range set this var
	
protected:
	//------------------------------Vars-----------------------------------
	// if Hybrid Approach -> blueprintReadWrite. If only C++ no need the BP macro
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,  Category="Targeting|Arrays")
	TArray<AActor*> ActorsInRange;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,  Category="Targeting|Arrays")
	TArray<AActor*> CyclingActors;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,  Category="Targeting|Arrays")
	TArray<AActor*> ActorsInSight;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Targeting|Visibility")
	bool bIsVisible;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Targeting|Arrays|Size")
	int32 ActorsInRangeSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Targeting|Arrays|Size")
	int32 CyclingActorsSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Targeting|Arrays|Size")
	int32 ActorsInSightSize;

	
private:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Targeting|Refs", meta=(AllowPrivateAccess="true"))
	TObjectPtr<APlayerController> PlayerControllerRef;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Targeting|Refs", meta=(AllowPrivateAccess="true"))
	TObjectPtr<ACharacter> PlayerCharacterRef;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	
	
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Hybrid / Helpers for Blueprints
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Getters", meta=(ArrayParm="TargetArray"))
	int32 GetMaxArrayNum(const TArray<AActor*>& TargetArray);
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Getters")
	int32 GetAllocatedSizeArray(const ETargetingPoolArraysType& ArrayType);
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Reserve")
	void ReserveTargetArray( UPARAM(ref) TArray<AActor*>& TargetArray, const int32 NumberToReserve);
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Reset")
	void ResetTargetArray( UPARAM(ref) TArray<AActor*>& TargetArray);
	
	// if UPARAM Does not work 
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Getters")
	int32 GetMaxArrayTypeNum(const ETargetingPoolArraysType& ArrayType);
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Reserve")
	void ReserveTypeArray( const ETargetingPoolArraysType& ArrayType,const int32 NumberToReserve);
	
	UFUNCTION(BlueprintCallable, Category= "TargetingFuncs|Hybrid|Reset")
	void ResetTypeArray( const ETargetingPoolArraysType& ArrayType);
	
	
	
	// Cycling Functions C++
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Sorting|C++")
	void FindActorsInRange();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Sorting|C++")
	void FindActorsInSight();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Sorting|C++")
	void ShowTarget();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Sorting|C++")
	void ClearTarget();
	
	UFUNCTION(BlueprintCallable,Category= "Targeting|Sorting|C++")
	void CancelTargeting();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Hybrid|Sort")
	void SortArray(UPARAM(ref) TArray<AActor*>& TargetArray);
	
	UFUNCTION()
	void Debug(const FString& String);
	
};


