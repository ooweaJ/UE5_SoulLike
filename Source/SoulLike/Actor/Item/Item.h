#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ActionData/ActionDataTableRow.h"
#include "GameplayTagContainer.h"
#include "Item.generated.h"

struct FActionData;
struct FItemInfoData;
struct FGameplayTag;

UCLASS()
class SOULLIKE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	void SetOwnerCharacter(ACharacter* InCharacter);
	FORCEINLINE class AAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE FActionData* GetCurrentData() { return &CurrentData; }
	FORCEINLINE class ACharacter* GetOwnerCharacter() { return OwnerCharacter; }


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void AddActionData();
	void CacheOwnerComponents();

	UFUNCTION()
	void OnRep_OwnerCharacter();

public:	
	virtual void Tick(float DeltaTime) override;
public:
	virtual FActionData* GetDefaultAction(uint32 Num = 1);
	virtual FActionData* GetDefaultAction2(uint32 Num = 1);
	virtual FActionData* GetSkillAction(uint32 Num = 1);
	virtual FActionData* GetUltimateAction();

	virtual void OnDefaultAction();
	virtual void OnDefaultAction2();
	virtual void OnDefaultAction3();
	virtual void OnSkillAction();
	virtual void OnSkillAction2();
	virtual void OnSkillAction3();
	virtual void OnUltimateAction();

	virtual void OffDefaultAction();
	virtual void OffDefaultAction2();
	virtual void OffDefaultAction3();
	virtual void OffSkillAction();
	virtual void OffSkillAction2();
	virtual void OffSkillAction3();
	virtual void OffUltimateAction();

	void SetupItemData();

	virtual void EndAction();
	virtual void ItemAction();
	virtual void ItemAction2();
	virtual void ItemAction3();
	virtual void OnItemSkillAction();
	virtual void OffItemSkillAction();

	void OnItemAction(int32 Num);

	void MontagePlayRate(UAnimInstance* AnimInstance, float PlayRate);
	void EndItem();
protected:
	UPROPERTY(EditAnywhere)
	class UCombatActionDataAsset* ItemData;
	UPROPERTY(ReplicatedUsing = "OnRep_OwnerCharacter")
	class ACharacter* OwnerCharacter;
protected:
	//UPROPERTY(Replicated)
	class AAttachment* Attachment;
	class UStateComponent* OwnerState;
	class UStatusComponent* OwnerStatus;
	TMap<FGameplayTag, FActionData> ActionTagMap;
	//UPROPERTY(Replicated)
	FActionData CurrentData;
	bool bItemDataSetup = false;
};
