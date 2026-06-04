#include "Notifies/ANS_Collision.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "Actor/Item/Attachment.h"

FString UANS_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UANS_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		if (AAttachment* Attachment = Weapon->GetAttachment())
		{
			if (FActionData* Data = Weapon->GetCurrentData())
			{
				if (Data->SpecificCollisionName == "None")
					Attachment->OnCollisions();
				else
					Attachment->OnCollisions(Data->SpecificCollisionName);
			}
		}
	}
}

void UANS_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ABaseWeapon* Weapon = GetCurrentWeapon(MeshComp))
	{
		if (AAttachment* Attachment = Weapon->GetAttachment())
		{
			Attachment->OffCollisions();
			Weapon->ReSetHitted();
		}
	}
}
