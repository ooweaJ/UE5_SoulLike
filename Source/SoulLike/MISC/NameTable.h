#pragma once

#include "CoreMinimal.h"

struct SOULLIKE_API FBlackBoardKeyNameTable
{
	static inline FName TargetKey = TEXT("Target");
	static inline FName LocationKey = TEXT("Location");
	static inline FName BehaviorKey = TEXT("Behavior");
	static inline FName RandomKey = TEXT("Random");
};

struct SOULLIKE_API FCollisionPresetNameTable
{
	static inline FName PhysicsActor = TEXT("PhysicsActor");
	static inline FName Player = TEXT("Player");
	static inline FName PlayerDetect = TEXT("PlayerDetect");
	static inline FName PlayerProjectile = TEXT("PlayerProjectile");
	static inline FName Enemy = TEXT("Enemy");
};