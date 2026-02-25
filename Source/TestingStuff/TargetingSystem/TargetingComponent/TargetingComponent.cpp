// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include "Engine/CoreSettings.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TestingStuff/TargetingSystem/TargetingInterface/TargetingInterface.h"
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
	PlayerControllerRef = Cast<APlayerController>(GetOwner());
	if (PlayerControllerRef)
	{
		PlayerCharacterRef = Cast<ACharacter>(PlayerControllerRef->GetPawn());
		ActorsInRange.Reserve(ActorsInRangeSize);
		ActorsInSight.Reserve(ActorsInSightSize);
		CyclingActors.Reserve(CyclingActorsSize);
	}
	
}

void UTargetingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

int32 UTargetingComponent::GetMaxArrayNum(const TArray<AActor*>& TargetArray)
{
	return TargetArray.Max();
}

int32 UTargetingComponent::GetAllocatedSizeArray(const ETargetingPoolArraysType& ArrayType)
{
	switch (ArrayType)
	{
	case ETargetingPoolArraysType::ActorsInRange:
		return ActorsInRange.GetAllocatedSize();
		
		
	case ETargetingPoolArraysType::CyclingActors:
		return CyclingActors.GetAllocatedSize();
		
	case ETargetingPoolArraysType::ActorsInSight:
		return ActorsInSight.GetAllocatedSize();
	default:
		return 0;
	}
	
}

void UTargetingComponent::ReserveTargetArray(TArray<AActor*>& TargetArray, const int32 NumberToReserve)
{
	TargetArray.Reserve(NumberToReserve);
}

void UTargetingComponent::ResetTargetArray(TArray<AActor*>& TargetArray)
{
	TargetArray.Reset();
}

int32 UTargetingComponent::GetMaxArrayTypeNum(const ETargetingPoolArraysType& ArrayType)
{
	switch (ArrayType)
	{
	case ETargetingPoolArraysType::ActorsInRange:
		return ActorsInRange.Max();
		
		
	case ETargetingPoolArraysType::CyclingActors:
		return CyclingActors.Max();
		
	case ETargetingPoolArraysType::ActorsInSight:
		return ActorsInSight.Max();
	default:
		return 0;
	}
}

void UTargetingComponent::ReserveTypeArray(const ETargetingPoolArraysType& ArrayType, const int32 NumberToReserve)
{
	switch (ArrayType)
	{
	case ETargetingPoolArraysType::ActorsInRange:
		ActorsInRange.Reserve(NumberToReserve);
		ActorsInRangeSize = NumberToReserve;
		break;
	case ETargetingPoolArraysType::CyclingActors:
		CyclingActors.Reserve(NumberToReserve);
		CyclingActorsSize = NumberToReserve;
		break;
	case ETargetingPoolArraysType::ActorsInSight:
		ActorsInSight.Reserve(NumberToReserve);
		ActorsInSightSize = NumberToReserve;
		break;
	}
}

void UTargetingComponent::ResetTypeArray(const ETargetingPoolArraysType& ArrayType)
{
	switch (ArrayType)
	{
	case ETargetingPoolArraysType::ActorsInRange:
		 ActorsInRange.Reset();
		 break;
		
	case ETargetingPoolArraysType::CyclingActors:
		 CyclingActors.Reset();
		 break;
	case ETargetingPoolArraysType::ActorsInSight:
		 ActorsInSight.Reset();
		 break;
	}
}

void UTargetingComponent::FindActorsInRange()
{
	if (PlayerCharacterRef)
	{
		ActorsInRange.Reset();
		
		const FVector& Location = PlayerCharacterRef->GetActorLocation();
		
		// Collision
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectParams;
		ObjectParams.Add(UEngineTypes::ConvertToObjectType(ECC_TARGETING));
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(PlayerCharacterRef);
		
		
		TArray<FHitResult> HitResults;
		
		
		bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Location,
			Location,
			TargetingRange,
			ObjectParams,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true);
		
		if (bHit)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AActor* Actor = HitResult.GetActor();
				
				// custom interface. Could cast to enemy base c++ as wel if 100% the only targets inherit from enemy base
				if (Actor && Actor->Implements<UTargetingInterface>())
				{
					const FTargetingData& Data = ITargetingInterface::Execute_GetTargetingData(Actor);
					if (Data.bIsTargetable && Data.EnemyTypeTag.MatchesAny(EnemyTypeTags)
						&& Data.EnemyStateTag.MatchesAny(StateTags)
						&& ActorsInRange.Num() < ActorsInRangeSize )
					{
						ActorsInRange.Add(Actor);
					}
					else
					{
						break;
					}
				}
			}
			if (ActorsInRange.Num() > 1)
			{
				SortArray(ActorsInRange);
			}
		}
	}
}

void UTargetingComponent::FindActorsInSight()
{
	if (PlayerControllerRef && !ActorsInSight.IsEmpty())
	{
		ActorsInSight.Reset();
		const FVector&  StartLocation = PlayerControllerRef->PlayerCameraManager->GetCameraLocation();
		
		// Collision
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(PlayerCharacterRef);
		
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_TARGETING);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		
		for (AActor*& Actor : ActorsInRange)
		{
			if (Actor && Actor->Implements<UTargetingInterface>())
			{
				const FVector& EndLocation = ITargetingInterface::Execute_GetTargetingPointLocation(Actor);
				FHitResult HitResult;
				bool bHit = GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				StartLocation,
				EndLocation,
				ObjectParams,
				QueryParams);
				
				if (HitResult.GetActor() && HitResult.GetActor() == Actor)
				{
					bIsVisible = true;
					if (!CyclingActors.Contains(Actor) && bIsVisible)
					{
						TargetActor = Actor;
						break;
					}
					if (bIsVisible && ActorsInSight.Num() < ActorsInSightSize)
					{
						ActorsInSight.Add(Actor);
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		Debug("No Enemies In Range");
	}
}

void UTargetingComponent::ShowTarget()
{
	if (IsValid(TargetActor) && TargetActor->Implements<UTargetingInterface>())
	{
		if (CyclingActors.Num() < CyclingActorsSize)
		{
			CyclingActors.Add(TargetActor);
		}
		else
		{
			CyclingActors.Reset();
		}
		ITargetingInterface::Execute_ShowTarget(TargetActor, true);
	}
	else
	{
		if (!ActorsInSight.IsEmpty())
		{
			CyclingActors.Reset();
			// Get The First valid actor and then break
			for (AActor*& Actor : ActorsInSight)
			{
				if (IsValid(Actor))
				{
					TargetActor = Actor;
					CyclingActors.Add(Actor); 
					ITargetingInterface::Execute_ShowTarget(TargetActor, true);
					break;
				}
			}
		}
	}
	if (CyclingActors.IsEmpty() && ActorsInSight.IsEmpty())
	{
		Debug("No Enemies Are In Sight");
	}
		
}

void UTargetingComponent::ClearTarget()
{
	if (IsValid(TargetActor) && TargetActor->Implements<UTargetingInterface>())
	{
		ITargetingInterface::Execute_ShowTarget(TargetActor, false);
		TargetActor = nullptr;
	}
}

void UTargetingComponent::CancelTargeting()
{
	ClearTarget();
	ActorsInRange.Reset();
	CyclingActors.Reset();
	ActorsInSight.Reset();
}

void UTargetingComponent::SortArray(TArray<AActor*>& TargetArray)
{
	if (PlayerCharacterRef)
	{
		const FVector& Loc = PlayerCharacterRef->GetActorLocation();
		
		TargetArray.Sort([Loc](const AActor& A, const AActor& B)
		{
			return FVector::DistSquared(A.GetActorLocation(), Loc)<
			FVector::DistSquared(B.GetActorLocation(),Loc);
		});
	}
}

void UTargetingComponent::Debug(const FString& String)
{
	{
		UKismetSystemLibrary::PrintString(GetWorld(), String, true,true, FColor::Purple, 5.f, NAME_None);
		
	}
}

// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

