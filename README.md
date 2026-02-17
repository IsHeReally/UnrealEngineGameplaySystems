
# Unreal Engine Gameplay Systems
##  Chapters :
1. [Few Words About Me](#few-words-about-me)
    - [Few Things About this Document](#few-things-about-this-document) 
2. [Targeting System](#targeting-system)
   - [Blueprint Approach](#blueprint-approach)
      - [Event Begin Play](#event-begin-play-blueprints)  
      - [GetActorsInRange](#get-actors-in-range-function)
      - [FilterEnemiesInSight](#filter-enemies-in-sight)
      - [GetClosestTarget](#get-closest-target)
      - [ShowTargetAndClear](#show-target-and-clear)
      - [CancelTargeting](#cancel-targeting)
      - [Problems](#problems)
      - [Result](#result)
   - [Hybrid Approach](#hybrid-approach)
      - [Array Functions](#array-functions)  
      - [Blueprint Logic](#blueprint-logic) 
#### Few Words About Me
---
    
Hello, my name is Charalampos and i am currently trying to transition from an Accounting and Finance backround, to create and design Gameplay Systems in Unreal Engine.

I’m not a professional programmer yet, but I understand basic **C++** and **Blueprint** scripting. I’m extremely curious and enjoy researching and building systems from scratch. I am currently document myself making several systems inside Unreal Engine here:

:point_right:  <https://www.youtube.com/@IsHeReally98> (ignore the poor quality :disappointed:). 

For now i've made some videos about :

 - Targeting System,
 - Input System 
 - Drop/Loot System (My own version & With Unreal Engine
Asset Manager) 
 - Inventory System 
 - Equipment
 - GAS 
 
 
I plan to document those systems as well, as much as i can, but my current goal is to learn not only how **Blueprints** interact with each other and **C++** but how can i optimize them as much as i can.

Anyways the point, of this document, or rather the theme of this document will be the **Targeting System**, since this was my first project as well.

#### Few Things About This Document

--- 

When I first started learning Unreal Engine, I relied heavily on courses and tutorials and fell into what many call *tutorial hell*. 

Over the last few months I decided to break away from copying and instead build my own systems. The targeting system here allows the player to cycle through targets by pressing a key. The implementation is presented in three stages:

- Blueprint-only approach 
- Hybrid Blueprint + C++ 
- Full C++ implementation 

The goal is to understand the logic, not just reproduce results. 
  
#### Targeting System 
--- 

What is a targeting system? 

Different genres approach targeting differently. A turn-based game, for example, selects a target directly through UI interaction, so there’s no need for runtime cycling or scanning. 


In real-time single-player or multiplayer games, however, the system must continuously compute valid targets around the player. That is where targeting logic becomes essential.

<p align="justify">
 But is the <b>Targeting System</b> even important to spend a considerable amount of time to build? Well in my personal opinion yes. While it highly depends on the game and of course the genre of the game, a good <b>Targeting System</b> contributes to not only the visuals but, also indicates to the player with who/what should interact. Let alone later in the game, could have a hybrid approach. For example, say you have a set of abilties. Some abilities might need a valid target to be activated. </p>

And since my own project, are based on RPG genre this **Targeting System** will be based on cycling, sorted by **Distance, Sight** and **Enemy Types**. 


## Blueprint Approach

--- 

<p align="justify">
My first ever video, of me documenting myself was based on a <b>Targeting System</b> that the Player could use, to cycle through different enemies. For example, i wanted the player to be able to target an Enemy, mostly an AI since this project focuses on single player perspective, and no matter the number of enemies in the scene, could cycle through and be sorted by <b>Distance, Sight</b> and <b>Enemy Types</b>. I actually managed to do that but now that i look back on how the project is, i feel like it's not only unoptimized and the scenario to be able to target a ''large'' amount of Enemies is not ideal. </p> 

:point_right: <https://www.youtube.com/watch?v=0PXnXTQs_8o&t=1265s> 

<p align="justify"> 
In the video above, i try my best to explain what i am essentially doing, but some <b>key parts</b> are, that i use local <b>Blueprint</b> TArray containers through the functions to collect correct data (Actor Pointers), then loop over them with the <b>for each Blueprint </b> node and sort them by <b>Sight</b> and then by <b>Distance</b>, using specifically the functions <b>Overlap Actors</b> and <b>Single Line Trace For Objects</b>. 
</p> 

#### Event Begin Play Blueprints

<p align="justify">
First things first. This is a <b>Blueprints</b> only approach. A key part of this specific component is that lives in the <b>PlayerController</b>. Now that this is out of the way, in <b>Event BeginPlay</b> can safely take the <b>GetOwner</b> node and do a Casting in order to have a controller ref and then from the controlled Pawn can Cast to <b>Character</b> and promote to a variable. This is relatively ''safe'' because the classes player and controller as far as i understand will be always in memory, no matter what. 
</p>



<p align="center">
<b>Event Begin Play</b>
</p>

<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\TargetingBlueprintsOnlyBeginPlay.PNG"></a>
<img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\TargetingBlueprintsOnlyBeginPlay.PNG" width="700">
</p>

---

Since this project is for single player though, i could use nodes like <b>GetPlayerController & GetPlayerCharacter</b> with index 0, which will return the local player.

<p align="center">
<b>Player Getters</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\CharactersIndex0.PNG"></a>
<img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\CharactersIndex0.PNG" width="700">
</p>

---

#### Get Actors In Range Function

<p align="justify">
We need in this component at least the character ref so in functions like <b>GetActorsInRange & GetFilterEnemiesInSight</b> and also <b>GetClosestTarget</b> so we have the location of the player. For the Function <b>GetActorsInRange</b> the idea is, when the player presses the key (for my own project i use the key Tab) it spawns an Overlapping Sphere for Actors with trace object channel of pawn(ECC_Pawn). Now though that i think about it, could use a multi sphere for objects and the object type could be my custom one.
</p>

<p align="center">
<b>Start of Actors In Range</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\OverlapWithPawns.PNG"></a>
<img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\OverlapWithPawns.PNG" width="700">
</p>

---

<p align="justify">
After the sphere spawn, i loop over the pawns that the sphere trace hit. I run checks like what is the <b>State Tag</b> (FGameplayTag inside the component and the enemy). A good example would be State.Dead, which if the enemy is dead the system has no reason to do other checks so it jumps to the next element which if the pawn passes the <b>State</b> condition can check a boolean that is indeed targetable but also can check the enemy type as well through agameplay tag. A good example would be if in the <b>SettingMenu UI</b> for Targeting, the Player could choose what type to target(Enemy.Type1 or Enemy.Type2). So, say, the player chooses Enemy.Type1, then the local array will have only Pawns that return the FGameplayTag Type2. But also if the player chooses Enemy (which is the parent/root tag), then every Pawn could get add to the local array.
</p>

<p align="center">
<b>Loop Over  and Filter The Enemy Pawns</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\LoopOverPawns.PNG"></a>
<img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\LoopOverPawns.PNG" width="700">

</p>


---

The Checks are coming from a blueprint interface that lives inside the enemy base, and has simple functions like:


- bIsTargetable(); which returns a boolean true or false

- GetStateTag(); which returns a state tag

- GetBoxCollisionLocation(); which returns the world location of a box collision above the Enemy AI head.

<p align="center">
<b>return if enemy pawn is targetable</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnbIsTargetable.PNG"></a>
  <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnbIsTargetable.PNG" width="700"/>
</p>


<p align="center">

|Return Target Location |Show Targeting Material/Visuals |
| --------------------- | ------------------------------ |
| <a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnBoxLocation.PNG"></a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnBoxLocation.PNG" width="500"/> |<a href="SScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ShowTargetingMaterial.PNG"></a> <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ShowTargetingMaterial.PNG" width="500"/> |

</p>

---

#### Filter Enemies In Sight

<p aligh="justify">
For the second function, in blueprints only approach, follows the <b>FilterEnemiesInSight</b>. What, is happenening is the system activates first the findsactorsinrange, which returns a local array. Then for filtering further more, by sight, the system loops over the local array. The difference is that, instead of a sphere for actors or multi sphere, I use a single line trace for objects, with <b>Object Types the World Static</b> and my own custom object type <b>Targeting</b> which is the EGameTraceChannel1(Unreal Engine gives us 19 custom channels and they called EGameTraceChannels). Also another difference is instead of spawning the line from the root of the player character, the starting location is from the camera. That helps the player and avoids the confusion that if the line trace hits a very small world static object in the world, even tho the enemy is visible, it won't target him. Further more, in the <b>Controller</b>, we could have a function that tweaks the camera Z position, so just to be safe i made a bool that if the camera is up in the Z Axis spawn the line trace from the current position, but if this boolean is false, spawn the line trace from the camera's current position + 200 units in Z axis (~2Meters).
</p>

<p align="center">
<b>Start Filtering By Sight</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\LineTraceAndCamera.PNG"></a>
  <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\LineTraceAndCamera.PNG" width="700"/>
</p>


---

<p align="justify">
I can do single line trace and not multi trace, because the system does a for each loop from the previous local array. Further more after i get the actor location, which is the interface that returns the world location from a box collision inside the enemy, if the line trace hits a world static object, the loop jumbs to the next element. If it did not hit a world static object then, the system checking again if what it hit is targetable and if it is, the element is getting added to another local array (Actors In Range). Now it's a good time to meet the main array of the component which i named it Actors. If the actor that the line trace that just hit, is inside the main array, the systems does nothing, instead it's jumping to the next element. But if the main array does not contain the item then it's getting added to a new local array (Fresh Actors In Range). On completion if the second local array is empty then , the main is getting cleared and then the first array (actors in range) is getting returned. If it's not empty , return the second local array </p>

|Trace Hit Conditions |Return Local Array |
|:-: | :-: |
| <a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\TraceHitCond.PNG"></a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\TraceHitCond.PNG" width="500"/> |<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnLocalCond.PNG"></a> <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\ReturnLocalCond.PNG" width="500"/> |
</p>


---

#### Get Closest Target

Okay, now that the system knows, the filtered actors in the world/scene, it's time to calculate the distance. For my own system, it's relatively the same for those tutorials. As you can see here: 

<p align="center">
<b>Calculation By Distance Inside the Loop</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\GetClosestTarget.PNG"></a>
<img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\BlueprintsOnly\GetClosestTarget.PNG" width="700">
</p>

---

<p align="justify">
I set two local variables. One is a ''bridge'' actor pointer and the other is a float. After i loop over the filtered array and calculate the distance using vector location, vector length, and returning the actor, so after can show the visuals. I tried nodes like <b>Distance(Vector)</b> but for some reason it was kinda buggy, or i was not using it correctly. An alternative choice could be the <b>DistanceSquared(Vector)</b>.
</p>



#### Show Target And Clear

Since those two functions are small, i decided to put those together in the [Chapters](#chapters-). The **clearing function** what it does, is set the main actor to nullptr and send a false as boolean with the interface to, ''hide/collapse'' any widgets or to set in my case the overlay material to nothing. On the other hand the **Show Target** is to set the main actor from the local variable, add to main array and then send a boolean as true to show the visuals.


|Clear Visuals | Show Closest Target  |
| :----------: | :---------: |
|<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\ClearVisuals.PNG"> </a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\ClearVisuals.PNG" width="500"> | <a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\ShowTargetActor.PNG"> </a> <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\ShowTargetActor.PNG" width="500">|

---

So now that the core functions are finished, the order of the activation would be pressing the key of choice -> clear visuals & setting the actor to nullptr -> Show Target Actor, which is triggering the function chain.


#### Cancel Targeting

One last thing before the problems section, is the cancel targeting function. With canceling the <b>Targeting System</b>, the system just calling the clearing visuals function to set the actor to nullptr and clearing the main array as well. This could take a separate key, of choice, and reset the whole targeting system.

<p align="center">
<b>CancelTargeting</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\CancelTargeting.PNG"> </a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\ScreenShots\CancelTargeting.PNG" width="700">
</p>

---

Some of these functions are irrelevant to the core targeting system and are just setting the character/pawn movement properties. Since the component lives in the controller though, those function could live in there instead of the component.


### Problems

Since this was my first project, I didn’t really dive too deep into optimization at the time. The system works, it cycles targets and filters by **Sight, Distance** and one thing I actually liked about the structure is that the local arrays are recreated every function call. That means they always start empty, so I never need to manually call **.Clear() Array**. Every execution is basically a fresh state.

But that “advantage” can become a problem really fast.

Right now I never check the length of the arrays anywhere. If a scene suddenly has 100 pawns and they all pass the conditions, they all get added and processed. In small encounters that’s fine, but imagine a war-type scene with lots of enemies and allies. Even actors that don’t end up being valid targets are still getting checked. The system technically works, but it doesn’t scale safely.

Another issue is memory behavior. From what I understood from Epic’s documentation <https://dev.epicgames.com/documentation/en-us/unreal-engine/array-containers-in-unreal-engine#t-array>, when .Add() or .AddUnique() is called, TArray doesn’t just add one element, it may reallocate and grow its capacity geometrically (similar to std::vector). This is efficient in general because it avoids constant resizing, but if the array keeps growing during loops it can still trigger extra allocations and copies.

A better approach would be to Reserve an expected size ahead of time:
```C++
    Array.Reserve(NumberToReserve);
```
This reduces reallocations and makes performance more predictable. In small scenes (10–30 pawns) this probably won’t matter much, but it’s still better practice to plan for growth instead of assuming the scene will always stay small. I could do probably something like that:

``` C++
 void ReserveArray(TArray<AActor*>& ArrayToReserve, int32 Num)
{
    ArrayToReserve.Reserve(Num); 
}
```
And pass the address of the blueprint array so i can reserve it but even with a UPARAM(ref) i see the TArray as an output and not as an input.

Another weakness is loop control. I never break out of loops early. So if 100 actors are added, the system checks all 100 every time even if a valid result was already found. When the player presses the targeting key, everything happens in one frame, which means the system could be doing unnecessary work.

There’s also a structural inefficiency in [Filter Enemies In Sight](#filter-enemies-in-sight). I use two local arrays that end up holding almost the same data. The first one mostly acts like a bridge, but it doesn’t actually need to store the full dataset. This creates extra allocations and memory churn that could be avoided with a cleaner data flow.

None of these issues break the system, but they show how a project that works functionally can still hide scaling problems once the number of actors increases.

Lastly, in the [Get Closest Target](#get-closest-target), I think it’s a waste of computation to keep calculating locations and comparing vectors manually inside loops. A cleaner approach would be to just sort the array once using predicate sorting in C++ and let the container handle ordering.

Something like this:

```C++
void SortArray()
{
  if(IsValid(PlayerCharacterRef))
  {
    FVector PlayerLocation = PlayerCharacterRef->GetActorLocation();
    Array.Sort([PlayerLocation](const AActor*& A, const AActor*& B)
    {
      return FVector::SquaredDist(A.GetActorLocation(), PlayerLocation)
            <FVector::SquaredDist(B.GetActorLocation(), PlayerLocation)
    });
  }
}
```
This way, instead of running a separate distance calculation function for every pawn each time I need the closest one, I can sort the array once after [Get Actors In Range](#get-actors-in-range-function) finishes and always have the closest target at index 0.

The distance checks still happen internally during sorting, but the logic becomes centralized and easier to reason about. It also removes the need for an extra pass over the array just to search for the nearest actor.

For small arrays the difference is probably negligible, but structurally this feels cleaner and easier to scale.

## Result

And since the **Blueprints** only approach is not in these files but the **Hybrid & C++** approach, i wanted to show with 3 simple small videos the results (With out any changes in the movement properties or locking the camera to hte target). If you want to see the functions either way, they are some screenshots in the Folder ScreenshotsAndVids->ScreenShots->TargetingSystem->BlueprintsOnly


## Hybrid Approach

Now that i ''finished'' talking about the [Blueprints Only Approach](#blueprint-approach), it's time to talk about the hybrid approach. In my opinion i find this way the best, especially since i do not fully understand **C++** but i get the aditional memory optimization functionality from **C++** and execute the logic with **Blueprints**. Also a very good point, is that **Blueprints** debugging and actually, see the flow (from one node to another), really helps me, to understand what the problems are and how i can fix them, while in **C++** i tried once to debug, and i gave up after 30 seconds.

Now there are some handy functions in **C++** that **Blueprints** does not expose, and from Epic's documentation it's recommended to reserve TArray Containers, especially when we know how elements could each hold <https://dev.epicgames.com/documentation/en-us/unreal-engine/array-containers-in-unreal-engine>. In this project files i will be using the names ***ActorsInRangePool, MainActorPool & ActorsInSightPool*** as those are the actual arrays in **C++**


### Array Functions

As i said above mostly in [Problems](#problems) section, even if in the game, you have a small amount of AI to target it's much preferable to .Reserve() and .Reset() instead of allocating and de allocating local arrays each time the player presses the key of choice, i could simply do something like this:

```C++
void ReserveArrays()
{
  ActorsInRangePool.Reserve(ActorsInRangePoolSize);
  MainActorPool.Reserve(MainActorPoolSize);
  ActorsInSightPool.Reserve(ActorsInSightPoolSize);
}
```

And call this function in **Event Begin Play** after the casting to get the PlayerControllerRef and the PlayerCharacterRef. The PoolSizes(ActorsInRangePoolSize  etc) i can give a UPROPERTY() macro with a **EditDefaults** only so not only it's friendly for the garbage collector, but i can tweak how much memory, i want to reserve in editor. A more dynamic way, if at some point accidentaly call .Clear() in some of the TArrays (because they are C++ variables if at any point i call clear i remove the Reserve function and the engine tries again to allocate and de allocate), would be something like this :

```C++
void ReserveArray(UPARAM(ref)TArray<AActor*>& ArrayToReserve, const int32 PoolSize)
{
  ArrayToReserve.Reserve(PoolSize);
}
```
That being said, because the TArray<AActor*>& is getting passed by reference, the editor might think it's an output and not an input. So the best way i found to be dynamic is something like this:
```C++
UENUM(BlueprintType)
enum class ETargetingPoolArraysType : uint8
{
	MainActorPool = 0  UMETA(DisplayName = "Main Actor Pool"),
	ActorsInRangePool  UMETA(DisplayName = "Actors In Range Pool"),
	ActorsInSightPool  UMETA(DisplayName = "Actors In Sight Pool"),
};
```
And i can make a functions like this :
```C++
void ReserveArray(const ETargetingPoolArraysType ArrayType, const int32 SizePool)
{
  switch(ArrayType)
  case  ETargetingPoolArraysType::MainActorPool:
        MainActorPool.Reserve(SizePool);
        MainActorPoolSize = SizePool;
        break;
        case  ETargetingPoolArraysType::ActorsInRangePool :
        ActorsInRangePool.Reserve(SizePool);
        ActorsInRangePoolSize = SizePool;
        break;
        case  ETargetingPoolArraysType::ActorsInSightPool :
        ActorsInSightPool.Reserve(SizePool);
        ActorsInSightPool = SizePool;
        break;
}

```
And for .Reset() function would be the same but the differense is, don't need to pass an integer to reset, since i want to Empty the TArray but i want the reserved memory to not change.

```C++
void ResetArray(const ETargetingPoolArraysType ArrayType)
{
  switch(ArrayType)
  case  ETargetingPoolArraysType::MainActorPool:
        MainActorPool.Reset();
        break;
        case  ETargetingPoolArraysType::ActorsInRangePool :
        ActorsInRangePool.Reset();
        break;
        case  ETargetingPoolArraysType::ActorsInSightPool :
        ActorsInSightPool.Reset();
        break;
}
```

<p align="justify">
Now, not only i can reserve the memory and make my CPU life more easy, but i can control how many iterations i want, and how i can approach the targeting. For example i could reserve for the ActorsInRangePool and MainActorPool for 100 pointes (in a 64x system this would be 200*8 bytes ~ 1,6kb), but i could also reserve 100 pointers for ActorsInRange and a much smaller amount for the Main Actor Pool, something like 20-30. With that logic, i could filter the 100 actors in the scene but the player could target only 20-30 closest targets which is a much more reasonable size(cause let's be real who will cycle over 100 targets?). </p>

I also can get helping function with the same logic of the UENUM, printing the allocated memory and max size in blueprints:

```C++
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
```

And lastly, instead of the [Get Closest Target](#get-closest-target), i can completely delete it and do something like this:
```C++
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
```
A few notes in the Sorting Predicate i'm not completely sure but because my TArrays are someting like this:
```C++
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
TArray<TObjectPtr<AActor>> MainActorPool;
	
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
TArray<TObjectPtr<AActor>> ActorsInRangePool;
	
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pool")
TArray<TObjectPtr<AActor>> ActorsInSightPool;
```

Inside the sorting, i need to use the same type of pointer, (i can't use a raw pointer AActor*& cause i am using TObjectPtr). That being said i get a warning which i do not know how to fix. A simpler approach would be, just to make the TArrays<AActor*> instead of TObjectPtrs.

Now that we finished with the **C++** functionality, in **Blueprints** those functions would look something like that:

<b>C++ Helper Functions</b>
</p>
<p align="center">
<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\C++FunctionsForBlueprints.PNG"></a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\C++FunctionsForBlueprints.PNG" width="800"></p>

---

#### Blueprint Logic

Since now i have the functions to help myself, i could use the same ''logic'' with the [Blueprints Only Approach](#blueprint-approach) but instead of emptying the arrays, or using locals i can reserve and reset, for efficiency. For the [Get Actors In Range](#get-actors-in-range-function), first get PlayerCharacterRef and PlayerControllerRef, convert to validate get, so only if are valid, can activate, remove the local array and use the ```TArray<TObjectPtr<AActor>> ActorInRangePool ``` , reset (emptying but keep the memory reserve) and do the same logic for the loop. I could also check if the .Num() is equal with ActorsInRangePoolSize, and if it's equal break the loop so i don't have dynamic allocations and re allocations. On completed, now i can sort the array and have it ready for the next function.

|Begin Sphere Overlap Actors | Loop Over Elements |
| :----------: | :---------: |
|<a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\HybridGetOverlapActors.PNG"> </a><img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\HybridGetOverlapActors.PNG" width="500"> | <a href="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\HybridLooping.PNG"> </a> <img src="ScreenShotsAndVids\ScreenShots\TargetingSystem\HybridApproach\HybridLooping.PNG" width="500">|
 