// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "SpearWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASpearWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ASpearWeapon(); 
	
	virtual void OnSkillAction();
	virtual void OnSkillAction2();

};
