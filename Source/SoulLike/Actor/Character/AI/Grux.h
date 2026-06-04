#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Grux.generated.h"

struct FActionData;

UCLASS()
class SOULLIKE_API AGrux : public AAIBaseCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime);

	void OnFlySkill(FActionData* InData);
	void OnSkill2();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SpawnActorsAround(float Distance, int32 NumberOfActors);

	UFUNCTION(Server,Reliable)
	void FinishFlySkill();
	UFUNCTION(NetMulticast,Reliable)
	void MultiFinishFlySkill();

	void ApproachTarget();

	UFUNCTION(Server, Reliable)
	void UpperSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiUpperSkill();

	UFUNCTION(BlueprintCallable)
	void AirStart();

	void PlayAirCombo();
	void AirDamage();
	UFUNCTION()
	void StartAirCombo();

	UFUNCTION()
	void EndAirCombo();

	UFUNCTION(NetMulticast, Reliable)
	void AirSpawnEffect(FTransform InTransform);
	UFUNCTION(NetMulticast, Reliable)
	void EndSkill2();
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AGruxMeteor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FlyAttack;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Skill2;

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bFly = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bTravel = false;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitEffect;

	bool bApproach = false;
	bool bSkill2 = false;

	TArray<class ABasePlayer*> HitPlayer;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	FVector TarGetLocation;
	FActionData* Data;

private:
	FVector CenterLocation;
	TArray<FVector> StarPoints;
	int32 CurrentPointIndex;
	float MoveSpeed = 5000.f;

	void DrawStarPattern(int Points, float Radius);
	void CalculateStarPoints(int Points, float Radius);
	void MoveToNextPoint();
};
