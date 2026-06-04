// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_EndAction.h"
#include "Actor/Character/BaseCharacter.h"

FString UAN_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;

	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		BaseCharacter->EndAction();
	}
}
