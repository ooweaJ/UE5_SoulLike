#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "BasePlayer.generated.h"

UCLASS()
class SOULLIKE_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABasePlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void OnMouseL();
	UFUNCTION(Server, Reliable)
	void OnMouseR();
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnMouseR();
	void OffMouseL();
	void OffMouseR();
	UFUNCTION(Server, Reliable)
	virtual void OnQ(); 
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnQ(); 
	UFUNCTION(Server, Reliable)
	void OnShift();
	UFUNCTION(Server, Reliable)
	void OffShift();
	UFUNCTION(Server, Reliable)
	void OnEvade();
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnEvade();
	UFUNCTION(Server, Reliable)
	void OnStepBack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnStepBack();
	void OnInteraction();
	void UsePotion();

	UFUNCTION(BlueprintCallable)
	void LockOn();
	
	void TickLockOn();
	virtual void UpdateHP();
	virtual void Dead();
	void HandlePlayerDeath(); 
	UFUNCTION(Server, Reliable)
	void CompletePlayerDeath();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCompletePlayerDeath(); 
	UFUNCTION(NetMulticast, Reliable)
	void MultiHandlePlayerRevival();
	UFUNCTION()
	virtual void HandlePlayerRevival(); 

	void SetPrimitiveComponentsVisibility(bool bVisible); 

	void SetAttachedActorsVisiblity(bool bVisible); 

	void DestroyAttachedActors(); 

	UFUNCTION(BlueprintCallable)
	void SetAirbone(bool InAirBone);

	UFUNCTION(BlueprintCallable)
	bool GetAbsolute();

public:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly)
	float ForwardInput;

	UPROPERTY(BlueprintReadOnly)
	float RightInput;

	UPROPERTY(BlueprintReadOnly)
	FVector MoveDirection;
	
	UPROPERTY(BlueprintReadOnly)
	float WalkingDirectionAngle;

	UPROPERTY(BlueprintReadWrite)
	bool bLockOn = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bEquipState;

	UPROPERTY(BlueprintReadWrite)
	float CharacterRotationAlphaLinearValue = 1;

	UPROPERTY(BlueprintReadWrite)
	float LeftHandWeight;

	UPROPERTY(BlueprintReadWrite)
	float UpperWeight;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float LockOnCameraArmLength = 300.0f;
	float LockOnRadius = 900;

	UPROPERTY(BlueprintReadWrite)
	float TESTNum = 0;

	UPROPERTY(BlueprintReadOnly,Replicated)
	bool bAirBone;


	bool Condition = true;
	
	FVector ArmPos;
	FVector ActorPos;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class AInteractiveActor* InteractableObject;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* DeathDissolveEffect; 

private:
	FTimerHandle PlayerRevivalTimerHandle;
	bool bDeathCompletionQueued = false;

};
