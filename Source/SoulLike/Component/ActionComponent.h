#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.generated.h"

USTRUCT()
struct FActiveAction
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag ActionTag;

	UPROPERTY()
	uint8 Sequence = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULLIKE_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

	void TryAction(const FGameplayTag& ActionTag);
	void EndPredictedAction();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Server, Reliable)
	void Server_TryAction(FGameplayTag ActionTag);

	UFUNCTION()
	void OnRep_ActiveAction();

	bool CanStartAction(const FGameplayTag& ActionTag) const;
	void CommitAction(const FGameplayTag& ActionTag);
	void PlayActionLocal(const FGameplayTag& ActionTag);
	void CacheOwnerComponents();
	bool IsOwnerMontagePlaying() const;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_ActiveAction")
	FActiveAction ActiveAction;

	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UEquipComponent* Equip;

	UPROPERTY()
	class UStateComponent* State;

	UPROPERTY()
	class UStatusComponent* Status;

	bool bPredictedActionInProgress = false;
};
