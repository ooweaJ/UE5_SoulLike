#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "GameplayEffect.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SOULLIKE_API ABaseCharacter : public ACharacter , public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	FORCEINLINE void SetParrying(bool InParry) { Parrying = InParry; }
	FORCEINLINE bool GetParrying() { return Parrying; }
	FORCEINLINE class UStateComponent* GetState() { return State; }
	FORCEINLINE class UEquipComponent* GetEquip() { return Equip; }
	FORCEINLINE class UStatusComponent* GetStatus() { return Status; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void EndAction();
	void HitPlayMontage(TSubclassOf<UDamageType> InDamageType);
	UFUNCTION(NetMulticast, Reliable)
	virtual void Dead();

	UFUNCTION(Server,Reliable)
	void SpawnBaseItem();
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AItem> DefaultItemClass;

	UPROPERTY(EditAnywhere)
	class UMontageComponent* MontageComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStateComponent* State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipComponent* Equip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStatusComponent* Status;

	bool Parrying;

public:
	FORCEINLINE bool IsRoll() { return bRoll; }
	bool bRoll = false;
};
