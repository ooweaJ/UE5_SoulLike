	#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Equip, Action, Hitted, Evade, StepBack, Dead, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULLIKE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsEvadeMode() { return Type == EStateType::Evade; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsStepBackMode() { return Type == EStateType::StepBack; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	bool IsCanCombo();

public:
	void SetIdleMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetEvadeMode();
	void SetStepBackMode();
	void SetDeadMode();
	void ChangeType(EStateType InNewType);

	void SetOnOrient();
	void SetOffOrient();

private:
	UPROPERTY(VisibleAnywhere)
	EStateType Type;
	class ACharacter* OwnerCharacter;
};
