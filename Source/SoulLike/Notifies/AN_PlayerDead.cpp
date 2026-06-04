// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_PlayerDead.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Actor/Controller/PlayerController/BasePlayerController.h"

void UAN_PlayerDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference); 

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(MeshComp->GetOwner()))
		{
				BasePlayer->CompletePlayerDeath();
		}
	}
}
