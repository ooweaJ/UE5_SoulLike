#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actor/Item/Item.h"
#include "Data/ActionData/ActionDataTableRow.h"
#include "CombatActionDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FItemInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UDataTable* Data;

	UPROPERTY(EditAnywhere, category = "Attach")
	TSubclassOf<class AAttachment> Attachment;

	UPROPERTY(EditAnywhere, category = "Attach")
	FName SocketName;

	UPROPERTY(EditAnywhere)
	FEquipmentData Equip;

	UPROPERTY(EditAnywhere)
	FEquipmentData UnEquip;
};


UCLASS()
class SOULLIKE_API UCombatActionDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemInfoData ItemInfoData;
};
