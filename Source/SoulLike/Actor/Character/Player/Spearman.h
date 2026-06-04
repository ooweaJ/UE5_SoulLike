// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Spearman.generated.h"

class USceneCaptureComponent2D; 
class ASpearProjectile; 
class ASpearWeapon;
class UCameraComponent; 

UCLASS()
class SOULLIKE_API ASpearman : public ABasePlayer
{
	GENERATED_BODY()

public:
	ASpearman(); 

public:
	ASpearProjectile* GetSpearProjectile() const { return SpearProjectile; }
	ASpearWeapon* GetSpearWeapon() const;
	UCameraComponent* GetCamera() const { return Camera; }

	UFUNCTION(NetMulticast, Reliable)
	void ThrowSpear();
	UPROPERTY(Replicated)
	bool bCanThrowSpear = false; 

protected:
	virtual void OnConstruction(const FTransform& Transform) override; 

	virtual void BeginPlay() override; 

	void SetupSpearProjectile(); 

	virtual void HandlePlayerRevival() override; 

	virtual void OnQ() override; 

	void ChangeBoolAtCooltimeOver(float CoolTime);

	FTimerHandle SpearSkillCoolHandle;


protected:
	ASpearProjectile* SpearProjectile = nullptr; 
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpearProjectile> Projecttileclass;
	bool bCanCombo = false; 

	UFUNCTION()
	void OnThrowSpearMontageEnded(UAnimMontage* Montage, bool bInterrupted); 	

	UPROPERTY(Replicated)
	bool bCanPlaySpearSkillMontage = true; 
	UPROPERTY(Replicated)
	FVector SpearControlVector;
};
