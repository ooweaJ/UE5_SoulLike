#include "Notifies/ANS_ItemSkill.h"
#include "Actor/Item/Weapon/BaseWeapon.h"

FString UANS_ItemSkill::GetNotifyName_Implementation() const
{
	return "ItemSkill";
}

void UANS_ItemSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		if (!Weapon->HasAuthority()) return;
		Weapon->OnItemSkillAction();
	}
}

void UANS_ItemSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		if (!Weapon->HasAuthority()) return;
		Weapon->OffItemSkillAction();
	}
}
