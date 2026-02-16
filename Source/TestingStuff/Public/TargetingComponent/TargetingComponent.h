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
	MainActorPool = 0  UMETA(DisplayName = "Main Actor Pool"),
	ActorsInRangePool  UMETA(DisplayName = "Actors In Range Pool"),
	ActorsInSightPool  UMETA(DisplayName = "Actors In Sight Pool"),
};


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class TESTINGSTUFF_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Actors|Main")
	TObjectPtr<AActor> MainValidActor;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//------------------------------Vars-----------------------------------
	//-----------------------------Arrays----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
	TArray<TObjectPtr<AActor>> MainActorPool;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
	TArray<TObjectPtr<AActor>> ActorsInRangePool;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
	TArray<TObjectPtr<AActor>> ActorsInSightPool; // Bridge Array. Does not need a big reserve
	
	//------------------------- End of Arrays ------------------------------
	
	// --------------------------BridgeActor -------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Actors|Bridge")
	TObjectPtr<AActor> BridgeActor;
	
	// -------------------------- End of Target Actor ------------------------
	
	//-------------------------------GameplayTags----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	TArray<FGameplayTag> StateTagArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Settings")
	FGameplayTag EnemySorting;
	//-----------------------------End of GameplayTags------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Visibility")
	bool bTargetIsVisible;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Visibility")
	float TargetingRange = 4000.f; // Default value
	//-----------------------------Pool Settings------------------------------
	
	
	// ---------------------------Pool Sizes ---------------------------------
	// Mainly if Arrays have different sizes. Can Tweak from Editor 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting|Pool|Size")
	int32 MainMaxPoolSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting|Pool|Size")
	int32 PoolSizeForActorsInRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting|Pool|Size")
	int32 PoolSizeForActorsInSight;
	
	//--------------------------End Of Pool Sizes-------------------------------
	//------------------------------End of Vars-------------------------------------------
	// Functions
	// --------------------------PoolSettings Functions---------------------------------
	UFUNCTION(BlueprintCallable, Category="Targeting|PoolSettings|Reserve")
	void ReserveArrays(const ETargetingPoolArraysType ArrayPool, int32 NumberToReserve);
	
	UFUNCTION(BlueprintCallable, Category="Targeting|PoolSettings|Reset")
	void ResetArrays(const ETargetingPoolArraysType ArrayPool);
	
	// Reserve 3 Arrays at once. Can do in BeginPlay without calling 3 times with different Enum.
	UFUNCTION(BlueprintCallable, Category = "Targeting|PoolSettings|Reserve")
	void ReserveActorArrays();
	// -------------End of Reserve And Reset Functions--------------------------
	
	// ------------------------Pool Sizes --------------------------------------
	// Can use the Get Max Size Of Arrays by each array or switch on Enums
	UFUNCTION(BlueprintPure, Category = "Targeting|Pool|Utility|Size")
	int32 GetMaxSizeOfArrays(const ETargetingPoolArraysType ArrayType) const;
	
	//----------------------End of PoolSizes------------------------------------
	
	//------------------------Memory Printing Function-------------------------------
	// Mainly to see memory size
	// Can use those functions either with enum or by each array
	UFUNCTION(BlueprintPure, Category = "Targeting|Pool|Utility|Memory")
	int32 GetAllocatedMemoryArrays(const ETargetingPoolArraysType ArrayTypes) const;
	//---------------End Memory Printing Functions---------------------------------
	
	//---------------------Targeting Functions-------------------------------------
	UFUNCTION(BlueprintCallable, Category= "Targeting|Functions|Sorting")
	void FindActorsInRange();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Functions|Sorting")
	void FilterActorsInRangeBySight();
	
	UFUNCTION(BlueprintCallable, Category ="Targeting|Functions|Sorting")
	void CheckFilteredArrays();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Functions|Sorting")
	void ClearVisualsFromTarget();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting|Functions|Sorting")
	void ShowVisualsOnTarget();
	// Find the Distance If Character Ref is valid and sort the ActorsInRange Array.
	UFUNCTION(BlueprintCallable, Category = "Targeting|Functions|Sorting")
	void SortActorsInRangeByDistance();
	// --------------------End Of Targeting Funcs------------------------------------
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Targeting|Refs", meta=(AllowPrivateAccess="true"))
	TObjectPtr<APlayerController> PlayerControllerRef;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Targeting|Refs", meta=(AllowPrivateAccess="true"))
	TObjectPtr<ACharacter> PlayerCharacterRef;
};


