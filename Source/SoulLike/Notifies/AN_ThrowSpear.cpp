// Fill out your copyright notice in the Description page of Project Settings.
#include "Notifies/AN_ThrowSpear.h"
#include "Actor/Character/Player/Spearman.h"
#include "Actor/Item/Weapon/SpearWeapon.h"

void UAN_ThrowSpear::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference); 

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ASpearman* Spearman = Cast<ASpearman>(MeshComp->GetOwner()))
		{
			Spearman->ThrowSpear(); 
		}
	}
}