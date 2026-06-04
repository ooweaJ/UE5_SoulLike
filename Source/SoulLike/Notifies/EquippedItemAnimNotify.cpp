#include "Notifies/EquippedItemAnimNotify.h"

#include "Actor/Item/Item.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "Component/EquipComponent.h"
#include "Components/SkeletalMeshComponent.h"

UEquipComponent* UEquippedItemAnimNotify::GetEquipComponent(const USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp || !MeshComp->GetOwner())
	{
		return nullptr;
	}

	return MeshComp->GetOwner()->FindComponentByClass<UEquipComponent>();
}

AItem* UEquippedItemAnimNotify::GetCurrentItem(const USkeletalMeshComponent* MeshComp) const
{
	if (UEquipComponent* Equip = GetEquipComponent(MeshComp))
	{
		return Equip->GetCurrentItem();
	}

	return nullptr;
}

ABaseWeapon* UEquippedItemAnimNotify::GetCurrentWeapon(const USkeletalMeshComponent* MeshComp) const
{
	return Cast<ABaseWeapon>(GetCurrentItem(MeshComp));
}
