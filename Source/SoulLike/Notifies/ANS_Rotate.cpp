// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ANS_Rotate.h"
#include "ANS_Rotate.h"
#include "Actor/Character/AI/AIBaseCharacter.h"

FString UANS_Rotate::GetNotifyName_Implementation() const
{
	return "Rotate";
}

void UANS_Rotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr) return;

	AAIBaseCharacter* Character = Cast<AAIBaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->SetRotate(true);
}

void UANS_Rotate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;

	AAIBaseCharacter* Character = Cast<AAIBaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->SetRotate(false);
}
