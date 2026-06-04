#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Actor/Item/DamageType/DefaultDamageType.h"
#include "GameplayTagContainer.h"
#include "Data/Item/ItemData.h"
#include "ActionDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
	FName StartSection;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bPawnControl = true;
};

USTRUCT(BlueprintType)
struct FActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditAnywhere)
	float Power = 1.0f;

	UPROPERTY(EditAnywhere)
	float HitStop = 1.0f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
	FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDamageType> DamageType = UDefaultDamageType::StaticClass();

	UPROPERTY(EditAnywhere)
	FString SpecificCollisionName = "None";

	UPROPERTY(EditAnywhere)
	bool bCanCombo = false;
};

USTRUCT(BlueprintType)
struct SOULLIKE_API FActionDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FActionData> ActionDatas;
};
