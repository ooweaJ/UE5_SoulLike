#include "Notifies/ANS_Combo.h"
#include "Actor/Item/Weapon/BaseWeapon.h"

FString UANS_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		Weapon->bCanCombo = true;
	}
}

void UANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		Weapon->bCanCombo = false;
	}
}
