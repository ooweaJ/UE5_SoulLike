#include "Component/MontageComponent.h"
#include "GameFramework/Character.h"
#include "Component/StatusComponent.h"

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

