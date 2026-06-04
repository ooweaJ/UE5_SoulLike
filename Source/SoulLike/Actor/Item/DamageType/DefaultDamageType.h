// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DefaultDamageType.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Default, Hitstop, Stun ,Knockback,Knockdown, Max,
};

UCLASS()
class SOULLIKE_API UDefaultDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EDamageType Type = EDamageType::Default;
};
