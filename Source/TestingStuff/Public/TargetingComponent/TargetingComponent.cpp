// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent/TargetingComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TestingStuff/TargetingComponent/TargetingInterface/TargetingInterface.h"
#define ECC_TARGETING ECC_GameTraceChannel1

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Can Set default values here for the max pool sizes or in editor
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	// Get owner which will be player controller and cast to get a ref. Also get the pawn cast to character
	// to have a character ref. 
	// Might need a delay at least in blueprints when call parent function from the child targeting component
	// This works only if this targeting component lives in Player Controller
	RetryNumber= 0;
	PlayerControllerRef = Cast<APlayerController>(GetOwner());
    if (PlayerControllerRef)
    {
	    PlayerCharacterRef = Cast<ACharacter>(PlayerControllerRef->GetPawn());
    	if (IsValid(PlayerCharacterRef))
    	{
    		// Reserve All Arrays. If reserving from blueprints then dont reserve here.
    		ReserveActorArrays();
    		GetWorld()->GetTimerManager().ClearTimer(CastTimer);
    	}
    }
    else
    {
    	GetWorld()->GetTimerManager().SetTimer(CastTimer,this,&UTargetingComponent::CastEveryXSeconds , 0.5f, true);
    	UE_LOG(LogTemp, Warning, TEXT("Owner Of TargetingComponent is not a player controller"));
    }
    
}
void UTargetingComponent::CastEveryXSeconds()
{
	RetryNumber++;
	if (RetryNumber > MaxRetries)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find PlayerController/Pawn after %d retries"), MaxRetries);
		GetWorld()->GetTimerManager().ClearTimer(CastTimer);
		return;
	}
	if (RetryNumber < MaxRetries && !IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = Cast<APlayerController>(GetOwner());
	}
	if (PlayerControllerRef && !IsValid(PlayerCharacterRef))
	{
		PlayerCharacterRef = Cast<ACharacter>(PlayerControllerRef->GetPawn());
	}
	if (IsValid(PlayerControllerRef) && IsValid(PlayerCharacterRef))
	{
		ReserveActorArrays();
		GetWorld()->GetTimerManager().ClearTimer(CastTimer);
		UE_LOG(LogTemp, Log, TEXT("PlayerController and Pawn found, arrays reserved."));
	}
}

void UTargetingComponent::ReserveActorArrays()
{
	// Reserve Arrays At Once in begin play
	ActorsInRangePool.Reserve(PoolSizeForActorsInRange);
	ActorsInSightPool.Reserve(PoolSizeForActorsInSight);
	MainActorPool.Reserve(MainMaxPoolSize);
}

void UTargetingComponent::ReserveArrays(const ETargetingPoolArraysType ArrayPool, int32 NumberToReserve)
{
	// Mostly if every in blueprints clear a specific array (.Empty()) can reserve a single Array with a
	// Number of choice. Since, in functions there is a check of TArray.Num() < PoolNumberSize ,
	// Set  Pool Sizes to be the number of the input. If this is on begin play remove reserveActorArrays()
	switch (ArrayPool)
	{
	case ETargetingPoolArraysType::MainActorPool:
		MainActorPool.Reserve(NumberToReserve);
		MainMaxPoolSize = NumberToReserve;
		break;
	case ETargetingPoolArraysType::ActorsInRangePool :
		ActorsInRangePool.Reserve(NumberToReserve);
		PoolSizeForActorsInRange = NumberToReserve;
		break;
	case ETargetingPoolArraysType::ActorsInSightPool:
		ActorsInSightPool.Reserve(NumberToReserve);
		PoolSizeForActorsInSight= NumberToReserve;
		break;
	}
}

void UTargetingComponent::ResetArrays(const ETargetingPoolArraysType ArrayPool)
{
	// Resetting the Array of choice. For Example in FindOverlapActors ResetArray(ETargetingPoolArraysType::ActorsInRange)
	// Can give default case and be Main Array 
	switch (ArrayPool)
	{
	case ETargetingPoolArraysType::MainActorPool :
		MainActorPool.Reset();
		break;
	case ETargetingPoolArraysType::ActorsInRangePool :
		ActorsInRangePool.Reset();
		break;
	case  ETargetingPoolArraysType::ActorsInSightPool:
		ActorsInSightPool.Reset();
		break;
	}
}

void UTargetingComponent::FindActorsInRange()
{
    // Could Check if the Array is not empty and if it's not dont loop over or reset the Array
	// Just either sort it again(Which will cause some goofy behavior) or do nothing, but
    // this would work only if the enemy number is fixed in the scene. If a new enemy appears
    // it won't detect. So could set a event by timer and every X seconds could reset the array,
	// to trigger the function again, but the behavior will be goofy as well.
	if (ActorsInRangePool.Num() < PoolSizeForActorsInRange)
	{
		ResetArrays(ETargetingPoolArraysType::ActorsInRangePool);
		// Check if controller and character refs are valid before activate the function.
		if (IsValid(PlayerCharacterRef) && IsValid(PlayerControllerRef))
		{
			FVector PlayerLocation = PlayerCharacterRef->GetActorLocation();
			TArray<AActor*> OverlapedActors; // local for Overlap Sphere Actors
		
			// Collision Settings 
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(PlayerCharacterRef); 
			ActorsToIgnore.Add(PlayerControllerRef);
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectParams;
		
			ObjectParams.Add(UEngineTypes::ConvertToObjectType(ECC_TARGETING));  // Custom Channel ECC_TARGETING = ECC_GameTraceChannel1
		
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
				PlayerLocation,
				TargetingRange,
				ObjectParams,
				nullptr,
				ActorsToIgnore,
				OverlapedActors);
		
			for (AActor* Actor : OverlapedActors)
			{
				// Check if the actor has the interface (Could check if Actor->Implements<UTargetingInterface>() first,but
				// it is the same thing I believe to check ITargetingInterface::Execute_IsTargetable(Actor) == true.
				// Also check the Tag Array. By default, in Blueprints the tags will be something like 
				// State.Dead, State.Invisible etc. If enemy has a tag like that, should not be added to ActorsInRange
				// Any Other Sorting Methods/Like Enemy Type Tags (Enemy.Monster.Type1 or Enemy.Monster.Type2 etc
				// can be checked here.
				if (IsValid(Actor) &&
					Actor->Implements<UTargetingInterface>())
				{
					if (ITargetingInterface::Execute_IsTargetable(Actor) == true
						&& ActorsInRangePool.Num() < PoolSizeForActorsInRange
						&& !StateTagArray.Contains(ITargetingInterface::Execute_GetStateTag(Actor)))
					{
						ActorsInRangePool.Add(Actor);
						if (ActorsInRangePool.Num() == PoolSizeForActorsInRange)
						{
							SortActorsInRangeByDistance();
							break;
						}
					}
				}
			}
		}
		else 
		{
			UE_LOG(LogTemp,Warning, TEXT("Player Character or Player Controller Refs are not valid."))
		}
	}
	//When Loop Finishes Sort The ActorsInRange Pool by distance using Predicate.
	if (!ActorsInRangePool.IsEmpty())
	{
		SortActorsInRangeByDistance();
	}
	
}

void UTargetingComponent::FilterActorsInRangeBySight()
{
	// Do this function only if the actors in range is not empty, and have something to loop over.
	if (!ActorsInRangePool.IsEmpty())
	{	
		ActorsInSightPool.Reset();
		// since this will run in the same frame, don't need to check for controller and player but 
		// doing it either way, just to be more safe.Also if Actors in Range is bigger than 0 ( not empty )
		if (IsValid(PlayerCharacterRef)&& IsValid(PlayerControllerRef) && ActorsInRangePool.Num() > 0)
		{
			// ---------------------Start Location for Line Trace-----------------------------------
			FVector StartLocation = PlayerControllerRef->PlayerCameraManager->GetCameraLocation();
			//Collision object Types. ECC_Targeting = ECC_GameTraceChannel1 
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_TARGETING);
			ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		
			FCollisionQueryParams ActorToIgnore;
			ActorToIgnore.AddIgnoredActor(PlayerCharacterRef);
			ActorToIgnore.AddIgnoredActor(PlayerControllerRef);
			//-----------------------------End Of Collision Params----------------------------------
		
			//----------------------------Loop over actors in range array----------------------------
			for (AActor* Actor : ActorsInRangePool)
			{
				if (IsValid(Actor) && Actor->Implements<UTargetingInterface>())
				{
					FHitResult OutHit;
					GetWorld()->LineTraceSingleByObjectType(OutHit,
					StartLocation,
					ITargetingInterface::Execute_GetTargetingPointLocation(Actor),
					ObjectParams,
					ActorToIgnore);
					//----------------------------Debugging Line Trace--------------------------------------
					//Draw Debug Line To See the trace
					DrawDebugLine(GetWorld(),
					StartLocation,
					ITargetingInterface::Execute_GetTargetingPointLocation(Actor),
					FColor::Red,
					false,
					1.f,
					0,
					1.f);
				
					DrawDebugSphere(GetWorld(),
					OutHit.ImpactPoint,
					20.f,
					5,
					FColor::Green,
					false,
					1.f,
					0,
					1.f);
					//----------------------------End of Debugging Line Trace--------------------------------------
				
					// Check if the actor that the line trace is the same as in the Actors in Range Array
					// Since the actors inside the Actors in Range pool are already sorted and passed the conditions
					// about the interface and state tags can safely execute. Maybe instead of MainActorPool TArray
					// could do MainActorPool TSet
					AActor* ActorHit = OutHit.GetActor();
					if (ActorHit == Actor)
					{
						bTargetIsVisible = true;
						ActorsInSightPool.Add(ActorHit);
						if (MainActorPool.Contains(ActorHit) && bTargetIsVisible)
						{
							ActorsInSightPool.Remove(ActorHit);
							BridgeActor = ActorHit;
						}
						else
						{
							//The first valid actor that finds, break the loop.
							MainValidActor = ActorHit;
							break;
						}
					}
					else
					{
						bTargetIsVisible = false;
					}
				}
			}
			// On Completed Check the Arrays when the loop finishes
			CheckFilteredArrays();
		}
	}
}

void UTargetingComponent::CheckFilteredArrays()
{
	// If the Bridge Array is empty and the main is also empty maybe the targeting did not
	// find anything, so if bridge actor is null. Very important to check since this could cause an 
	// infinate loop
	if (ActorsInSightPool.IsEmpty())
	{
		if (MainActorPool.IsEmpty())
		{
			if (BridgeActor == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No Valid/Visible/Targets in Sigh/Range"));
			}
			else
			{
				// if bridge actor is valid then set Main Valid Actor.
				MainValidActor = BridgeActor;
			}
		}
		else
		{
			// If the Bridge TArray(ActorsInSight) is empty but the main is not,
			// check the last item, since this will be the closest to the player
			// if valid remove it from the actors in range pool, reset main and
			// re-run the filter, if not valid dont remove it from the actors in
			// reset main and rerun.
			
			AActor* ActorToRemove = MainActorPool.Last();
			if (IsValid(ActorToRemove))
			{
				ActorsInRangePool.Remove(ActorToRemove);
				
			}
			ResetArrays(ETargetingPoolArraysType::MainActorPool);
			FilterActorsInRangeBySight();
		}
	}
}

void UTargetingComponent::ClearVisualsFromTarget()
{
	// Clear the visuals with the Interface and set the bridge and main actor to null
	if (IsValid(MainValidActor) && MainValidActor->Implements<UTargetingInterface>())
	{
		ITargetingInterface::Execute_ShowTarget(MainValidActor, false);
		MainValidActor = nullptr;
		BridgeActor = nullptr;
	}
}

void UTargetingComponent::ShowVisualsOnTarget()
{
	// Order of the Target Begin (From the controller or inside the component) will be
	// ClearVisualsFromTarget(); ShowVisualsOnTarget(); 
	// The two Functions Run to find either a valid target actor closest and visible,
	// or dont find anything.
	FindActorsInRange();
	FilterActorsInRangeBySight();
	if (IsValid(MainValidActor) && MainValidActor -> Implements<UTargetingInterface>())
	{
		if (MainActorPool.Num()<MainMaxPoolSize)
		{
			MainActorPool.Add(MainValidActor);
			ITargetingInterface::Execute_ShowTarget(MainValidActor,true);
		}
		else
		{
			ResetArrays(ETargetingPoolArraysType::MainActorPool);
			FilterActorsInRangeBySight();
			if (IsValid(MainValidActor) && MainValidActor->Implements<UTargetingInterface>())
			{
				MainActorPool.Add(MainValidActor);
				ITargetingInterface::Execute_ShowTarget(MainValidActor, true);
			}
		}
	}
}

void UTargetingComponent::SortActorsInRangeByDistance()
{
	// if valid then sort the array.
	if (IsValid(PlayerCharacterRef))
	{
		FVector PlayerLocation = PlayerCharacterRef->GetActorLocation();
		
		ActorsInRangePool.Sort([PlayerLocation](const TObjectPtr<AActor>& A, const TObjectPtr<AActor>& B)
		{
		
			return FVector::DistSquared(A.Get()->GetActorLocation(), PlayerLocation) < FVector::DistSquared(B.Get()->GetActorLocation(),PlayerLocation);
		});
	}
}



// Mostly Debugging Functions For Blueprints.
int32 UTargetingComponent::GetMaxSizeOfArrays(const ETargetingPoolArraysType ArrayType) const
{
	// Returns the Max Number. Mostly a helper Function for debugging in blueprints
	switch (ArrayType)
	{
	case ETargetingPoolArraysType::MainActorPool:
		return MainActorPool.Max();
		
	case ETargetingPoolArraysType::ActorsInRangePool:
		return ActorsInRangePool.Max();
		
	case ETargetingPoolArraysType::ActorsInSightPool:
		return ActorsInSightPool.Max();
		
	default:
		return 0;
	}
}

int32 UTargetingComponent::GetAllocatedMemoryArrays(const ETargetingPoolArraysType ArrayTypes) const
{
	// Mostly to see the memory that arrays using, inside blueprints
	switch (ArrayTypes)
	{
	case ETargetingPoolArraysType::MainActorPool:
		return MainActorPool.GetAllocatedSize();
		
	case ETargetingPoolArraysType::ActorsInRangePool:
		return ActorsInRangePool.GetAllocatedSize();
		
	case ETargetingPoolArraysType::ActorsInSightPool:
		return ActorsInSightPool.GetAllocatedSize();
	default:
		return 0;
	}
}

// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

