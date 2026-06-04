// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipComponent.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FPotion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULLIKE_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE AItem* GetCurrentItem() { return SelectItem; }
	FORCEINLINE void SetSelectItem(AItem* InItem) { SelectItem = InItem; }
	void EndAction();

	// Potion
	FORCEINLINE bool CanUsePotion() { return Potion.Amount > 0; }
	FORCEINLINE void UsePotion() { Potion.Amount--; }
	FORCEINLINE void SupplyPotion() { Potion.Amount = 10; }
	FORCEINLINE void SetPotionHealAmount(float InAmount) { Potion.HealAmount = InAmount; }
	FORCEINLINE float GetPotionHealAmount() { return Potion.HealAmount; }
	FORCEINLINE FPotion* GetPotion() { return &Potion; }

private:
	TArray<AItem*> EquipItems;
	//UPROPERTY(Replicated)
	AItem* SelectItem;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPotion Potion;
};
