// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_StaminaCost.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Component/StatusComponent.h"

void UAN_StaminaCost::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) return;

	if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(MeshComp->GetOwner()))
	{
		UStatusComponent* Status = BasePlayer->GetStatus();
		float AttackCost = Status->GetAttackCost();

		Status->StatusModify(Status->SP, AttackCost);
	}
}