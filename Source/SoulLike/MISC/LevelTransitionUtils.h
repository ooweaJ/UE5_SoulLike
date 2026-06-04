// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SOULLIKE_API LevelTransitionUtils
{
public:
	static void OpenLevelWithData(UObject* WorldContextObject, const FString& LevelName, const struct FCharacterData& Data);
};
