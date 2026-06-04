#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "AIBaseCharacter.generated.h"

UCLASS()
class SOULLIKE_API AAIBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAIBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server, Reliable)
	virtual void OnMelee(uint32 Num = 1);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiOnMelee(uint32 Num = 1);
	UFUNCTION(Server, Reliable)
	virtual void OnSkill(uint32 Num = 1);
	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiOnSkill(uint32 Num = 1);

	virtual void OnUltimate();
	virtual void OffMelee();
	virtual void OffSkill();
	virtual void OffUltimate();
	virtual void Dead();
	UFUNCTION()
	void End_Dead();

	void TargetRotation();
	void RotateToTarget();
	virtual void UpdateHP();

	UFUNCTION(Server, Reliable)
	void PlayRateMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayRateMontage();

	UFUNCTION()
	void OnUIPopUP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnUIOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE void SetMoveDirection(FVector InDirection) { Direction = InDirection; }
	FORCEINLINE void SetStrafe(bool InStrafe) { bStrafe = InStrafe; }
	FORCEINLINE void SetRotate(bool InRotate) { bRotate = InRotate; }

protected:
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* HealthWidget;
	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> StatusWidgetClass;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName NameTag;
protected:
	class ABaseAIController* BaseController;

	UPROPERTY(EditAnywhere)
	class USphereComponent* UIPopCollision;

protected:
	FVector Direction;
	bool bStrafe = false;
	bool bRotate = false;
	FVector TargetLocation;

	class UUI_BossStatus* ScreenStatusUI;
};
