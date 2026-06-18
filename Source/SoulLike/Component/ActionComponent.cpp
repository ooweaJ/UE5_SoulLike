#include "Component/ActionComponent.h"
#include "Actor/Item/Item.h"
#include "BaseGameplayTags.h"
#include "Component/EquipComponent.h"
#include "Component/StateComponent.h"
#include "Component/StatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwnerComponents();
}

void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UActionComponent, ActiveAction, COND_SkipOwner);
}

void UActionComponent::TryAction(const FGameplayTag& ActionTag)
{
	if (!ActionTag.IsValid()) return;
	CacheOwnerComponents();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CommitAction(ActionTag);
		return;
	}

	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled() && CanStartAction(ActionTag))
	{
		PlayActionLocal(ActionTag);
		bPredictedActionInProgress = true;
	}

	Server_TryAction(ActionTag);
}

void UActionComponent::EndPredictedAction()
{
	bPredictedActionInProgress = false;
}

void UActionComponent::Server_TryAction_Implementation(FGameplayTag ActionTag)
{
	CacheOwnerComponents();
	CommitAction(ActionTag);
}

void UActionComponent::OnRep_ActiveAction()
{
	PlayActionLocal(ActiveAction.ActionTag);
}

bool UActionComponent::CanStartAction(const FGameplayTag& ActionTag) const
{
	if (!ActionTag.IsValid()) return false;
	if (!Equip || !State || !Status) return false;
	if (!Equip->GetCurrentItem()) return false;
	if (ActionTag != BaseGameplayTags::Ability_DefaultAction) return false;
	if (Status->SP.Current < -Status->GetAttackCost()) return false;

	if (State->IsCanCombo()) return true;

	if (State->IsIdleMode())
	{
		return !bPredictedActionInProgress || !IsOwnerMontagePlaying();
	}

	if (State->IsActionMode() && !IsOwnerMontagePlaying())
	{
		return true;
	}

	return false;
}

void UActionComponent::CommitAction(const FGameplayTag& ActionTag)
{
	if (!CanStartAction(ActionTag)) return;

	PlayActionLocal(ActionTag);

	ActiveAction.ActionTag = ActionTag;
	++ActiveAction.Sequence;

	if (AActor* Owner = GetOwner())
	{
		Owner->ForceNetUpdate();
	}
}

void UActionComponent::PlayActionLocal(const FGameplayTag& ActionTag)
{
	if (!Equip)
	{
		CacheOwnerComponents();
	}

	AItem* Item = Equip ? Equip->GetCurrentItem() : nullptr;
	if (!Item) return;

	if (ActionTag == BaseGameplayTags::Ability_DefaultAction)
	{
		Item->OnDefaultAction();
	}
}

void UActionComponent::CacheOwnerComponents()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	Equip = OwnerCharacter->GetComponentByClass<UEquipComponent>();
	State = OwnerCharacter->GetComponentByClass<UStateComponent>();
	Status = OwnerCharacter->GetComponentByClass<UStatusComponent>();
}

bool UActionComponent::IsOwnerMontagePlaying() const
{
	if (!OwnerCharacter) return false;

	const USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return false;

	const UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	return AnimInstance && AnimInstance->IsAnyMontagePlaying();
}
