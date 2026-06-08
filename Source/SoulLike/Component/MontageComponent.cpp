#include "Component/MontageComponent.h"
#include "GameFramework/Character.h"
#include "Component/StatusComponent.h"
#include "Actor/Item/DamageType/DefaultDamageType.h"

UMontageComponent::UMontageComponent()
{
}

void UMontageComponent::PlayKnockBack()
{
	PlayAnimMontage("KnockBack");
}

void UMontageComponent::PlayKnockDown()
{
	PlayAnimMontage("KnockDown");
}

void UMontageComponent::PlayAvoid()
{
	PlayAnimMontage("Avoid");
}

void UMontageComponent::PlayRoll()
{
	PlayAnimMontage("Roll");
}

void UMontageComponent::PlayHit()
{
	PlayAnimMontage("Hit");
}

void UMontageComponent::PlayStun()
{
	PlayAnimMontage("Stun");
}

void UMontageComponent::PlayDead()
{
	PlayAnimMontage("Dead");
}

void UMontageComponent::PlayHitReaction(TSubclassOf<UDamageType> InDamageType)
{
	const FName MontageKey = GetHitReactionMontageKey(InDamageType);
	if (MontageKey.IsNone()) return;

	PlayAnimMontage(MontageKey);
}

FName UMontageComponent::GetHitReactionMontageKey(TSubclassOf<UDamageType> InDamageType) const
{
	if (!InDamageType) return NAME_None;

	const UDefaultDamageType* DamageType = Cast<UDefaultDamageType>(InDamageType->GetDefaultObject());
	if (!DamageType) return NAME_None;

	switch (DamageType->Type)
	{
	case EDamageType::Default:
		return NAME_None;
	case EDamageType::Hitstop:
		return "Hit";
	case EDamageType::Stun:
		return "Stun";
	case EDamageType::Knockback:
		return "KnockBack";
	case EDamageType::Knockdown:
		return "KnockDown";
	default:
		return NAME_None;
	}
}

void UMontageComponent::PlayAnimMontage(FName Key)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	if (!MontageData) return;
	FMontageData* data = MontageData->FindRow<FMontageData>(Key, "");
	if (!data) return ;

	UStateComponent* state = character->GetComponentByClass<UStateComponent>();
	UStatusComponent* status = character->GetComponentByClass<UStatusComponent>();

	if (!data->AnimMontage) return;
	character->StopAnimMontage();
	state->ChangeType(data->Type);
	data->bCanMove ? status->SetMove() : status->SetStop();
	character->PlayAnimMontage(data->AnimMontage, data->PlayRate, data->StartSection);
}

