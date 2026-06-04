#include "StateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/EquipComponent.h"
#include "Actor/Item/Weapon/BaseWeapon.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UStateComponent::SetStepBackMode()
{
	ChangeType(EStateType::StepBack);
}

void UStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UStateComponent::SetOnOrient()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UStateComponent::SetOffOrient()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;
}

bool UStateComponent::IsCanCombo()
{
	UEquipComponent* equip = OwnerCharacter->GetComponentByClass<UEquipComponent>();
	if (!equip) return false;

	ABaseWeapon* weapon = Cast<ABaseWeapon>(equip->GetCurrentItem());
	if (!weapon) return false;

	return weapon->IsCanCombo();
}