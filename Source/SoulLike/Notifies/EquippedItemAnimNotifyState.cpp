#include "Notifies/EquippedItemAnimNotifyState.h"

#include "Actor/Item/Item.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "Component/EquipComponent.h"
#include "Components/SkeletalMeshComponent.h"

UEquipComponent* UEquippedItemAnimNotifyState::GetEquipComponent(const USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp || !MeshComp->GetOwner())
	{
		return nullptr;
	}

	return MeshComp->GetOwner()->FindComponentByClass<UEquipComponent>();
}

AItem* UEquippedItemAnimNotifyState::GetCurrentItem(const USkeletalMeshComponent* MeshComp) const
{
	if (UEquipComponent* Equip = GetEquipComponent(MeshComp))
	{
		return Equip->GetCurrentItem();
	}

	return nullptr;
}

ABaseWeapon* UEquippedItemAnimNotifyState::GetCurrentWeapon(const USkeletalMeshComponent* MeshComp) const
{
	return Cast<ABaseWeapon>(GetCurrentItem(MeshComp));
}
