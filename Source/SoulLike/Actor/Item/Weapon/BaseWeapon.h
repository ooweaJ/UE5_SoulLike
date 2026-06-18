// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API ABaseWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();

	virtual void BeginPlay() override;

	FORCEINLINE void ReSetHitted() { HittedCharacters.Empty(); }
	FORCEINLINE bool IsCanCombo() { return bCanCombo; }

	UFUNCTION()
	virtual void OnDamage(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter);
	virtual void OnDefaultAction();
	virtual void EndAction();
	virtual void ItemAction();
	virtual void ItemAction2();
public:
	TArray<class ACharacter*> HittedCharacters;
	float WeaponDamage = 100.f;
	float AttackSpeed = 1.f;
	uint32 ComboCount = 1;
	bool bCanCombo = false;
	bool bSucceed = false;
};
