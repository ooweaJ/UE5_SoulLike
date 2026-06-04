#include "Actor/Item/WarriorItem.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Actor/Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Actor/Item/DamageType/DefaultDamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Item/Attachment.h"

void AWarriorItem::OnDamage(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	if (Parry)
	{
		 ABaseCharacter* Target = Cast<ABaseCharacter>(InOtherCharacter);
		 if (Target)
		 {
			 if (Target->GetParrying())
			 {
				 if (!DDT) return;
				 FDamageEvent de;
				 de.DamageTypeClass = DDT;
				 InOtherCharacter->TakeDamage(0, de, InAttacker->GetController(), InCauser);
				 //Effect
				 UParticleSystem* hitEffect = CurrentData.Effect;
				 if (!!hitEffect)
				 {
					 FTransform transform = CurrentData.EffectTransform;
					 transform.AddToTranslation(Attachment->GetActorLocation());
					 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
				 }
			 }
		 }
	}
	else
	{
		Super::OnDamage(InAttacker,InCauser,InOtherCharacter);
	}
}

void AWarriorItem::OnDefaultAction()
{
	Super::OnDefaultAction();
}

void AWarriorItem::OnDefaultAction2()
{
}

void AWarriorItem::OnDefaultAction3()
{
}

void AWarriorItem::OnSkillAction()
{
	Super::OnSkillAction();
}

void AWarriorItem::OnSkillAction2()
{
}

void AWarriorItem::OnSkillAction3()
{
}

void AWarriorItem::OnUltimateAction()
{
}

void AWarriorItem::OffDefaultAction()
{

}

void AWarriorItem::OffDefaultAction2()
{
}

void AWarriorItem::OffDefaultAction3()
{
}

void AWarriorItem::OffSkillAction()
{
	if (OwnerCharacter)
	{
		if (ABasePlayer* Player = Cast<ABasePlayer>(OwnerCharacter))
		{
			Player->UpperWeight = 0;
		}
	}
}

void AWarriorItem::OffSkillAction2()
{
}

void AWarriorItem::OffSkillAction3()
{
}

void AWarriorItem::OffUltimateAction()
{
}

void AWarriorItem::OnItemSkillAction()
{
	Parry = true;
}

void AWarriorItem::OffItemSkillAction()
{
	Parry = false;
}
