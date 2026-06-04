// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor/Item/Weapon/SpearWeapon.h"
#include "Data/ActionData/CombatActionDataAsset.h"
#include "Actor/Character/Player/Spearman.h"
#include "Actor/Item/Weapon/SpearProjectile.h"
#include "Actor/Item/Attachment.h"
#include "Component/StateComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASpearWeapon::ASpearWeapon()
{
	{
		static ConstructorHelpers::FObjectFinder<UCombatActionDataAsset> DA(TEXT("/Script/SoulLike.CombatActionDataAsset'/Game/_dev/Data/DA/DA_Spearman.DA_Spearman'"));
		ensure(DA.Object); 
		ItemData = DA.Object; 
	}	
}


void ASpearWeapon::OnSkillAction()
{
	ACharacter* CharacterOwner = GetOwnerCharacter();
	if (ASpearman* SpearOwner = Cast<ASpearman>(CharacterOwner))
	{
		// Play AnimMontage
		FActionData* Data = GetSkillAction(2);
		if (!Data) { return; }


		SpearOwner->PlayAnimMontage(Data->AnimMontage, 1.f);
		OwnerState->SetIdleMode();
	}
}

void ASpearWeapon::OnSkillAction2()
{
	ACharacter* CharacterOwner = GetOwnerCharacter();
	if (ASpearman* SpearOwner = Cast<ASpearman>(CharacterOwner))
	{
		// Visibility
		ASpearProjectile* SpearProjectile = SpearOwner->GetSpearProjectile();
		SpearProjectile->SetComponentsVisibility(true);
		Attachment->SetActorHiddenInGame(true);

		// Play AnimMontage
		FActionData* Data = GetSkillAction(1);
		if (!Data) { return; }


		SpearOwner->PlayAnimMontage(Data->AnimMontage, 1.25f);
		OwnerState->SetActionMode();
	}
}

