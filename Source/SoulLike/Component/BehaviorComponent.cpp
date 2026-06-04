#include "Component/BehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFrameWork/Character.h"
#include "Misc/MISC.h"

UBehaviorComponent::UBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UBehaviorComponent::IsWaitMode() { return GetType() == EBehaviorType::Wait; }
bool UBehaviorComponent::IsApproachMode() { return GetType() == EBehaviorType::Approach; }
bool UBehaviorComponent::IsMeleeMode() { return GetType() == EBehaviorType::Melee; }
bool UBehaviorComponent::IsSkillMode() { return GetType() == EBehaviorType::Skill; }
bool UBehaviorComponent::IsUltimateMode() { return GetType() == EBehaviorType::Ultimate; }
bool UBehaviorComponent::IsPatrolMode() { return GetType() == EBehaviorType::patrol; }
bool UBehaviorComponent::IsHittedMode() { return GetType() == EBehaviorType::Hitted; }
bool UBehaviorComponent::IsKnockBackMode() { return GetType() == EBehaviorType::KnockBack; }
bool UBehaviorComponent::IsStrafeMode() { return GetType() == EBehaviorType::Strafe; }

ACharacter* UBehaviorComponent::GetTarget()
{
	ACharacter* character = Cast<ACharacter>(Blackboard->GetValueAsObject(FBlackBoardKeyNameTable::TargetKey));
	if (!!character)
		return character;
	return nullptr;
}

FVector UBehaviorComponent::GetLocation()
{
	return Blackboard->GetValueAsVector(FBlackBoardKeyNameTable::LocationKey);
}

void UBehaviorComponent::SetWaitMode() { ChangeType(EBehaviorType::Wait); }
void UBehaviorComponent::SetApproachMode() { ChangeType(EBehaviorType::Approach); }
void UBehaviorComponent::SetMeleeMode() { ChangeType(EBehaviorType::Melee); }
void UBehaviorComponent::SetSkillMode() { ChangeType(EBehaviorType::Skill); }
void UBehaviorComponent::SetUltimateMode() { ChangeType(EBehaviorType::Ultimate); }
void UBehaviorComponent::SetPatrolMode() { ChangeType(EBehaviorType::patrol); }
void UBehaviorComponent::SetHittedMode() { ChangeType(EBehaviorType::Hitted); }
void UBehaviorComponent::SetKnockBackMode() { ChangeType(EBehaviorType::KnockBack); }
void UBehaviorComponent::SetEquipMode() { ChangeType(EBehaviorType::Equip); }
void UBehaviorComponent::SetStrafeMode() { ChangeType(EBehaviorType::Strafe); }
void UBehaviorComponent::SetDeadMode() { ChangeType(EBehaviorType::Dead); }

void UBehaviorComponent::ChangeType(EBehaviorType InType)
{
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(FBlackBoardKeyNameTable::BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(InType);
}

EBehaviorType UBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(FBlackBoardKeyNameTable::BehaviorKey);
}

