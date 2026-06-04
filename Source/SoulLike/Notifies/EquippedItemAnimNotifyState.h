#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EquippedItemAnimNotifyState.generated.h"

class ABaseWeapon;
class AItem;
class UEquipComponent;

UCLASS(Abstract)
class SOULLIKE_API UEquippedItemAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UEquipComponent* GetEquipComponent(const USkeletalMeshComponent* MeshComp) const;
	AItem* GetCurrentItem(const USkeletalMeshComponent* MeshComp) const;
	ABaseWeapon* GetCurrentWeapon(const USkeletalMeshComponent* MeshComp) const;
};
