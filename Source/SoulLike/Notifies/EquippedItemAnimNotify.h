#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EquippedItemAnimNotify.generated.h"

class ABaseWeapon;
class AItem;
class UEquipComponent;

UCLASS(Abstract)
class SOULLIKE_API UEquippedItemAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UEquipComponent* GetEquipComponent(const USkeletalMeshComponent* MeshComp) const;
	AItem* GetCurrentItem(const USkeletalMeshComponent* MeshComp) const;
	ABaseWeapon* GetCurrentWeapon(const USkeletalMeshComponent* MeshComp) const;
};
