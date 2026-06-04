// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ActionData/ActionDataTableRow.h"
#include "GruxMeteor.generated.h"

UCLASS()
class SOULLIKE_API AGruxMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGruxMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetTarget(AActor* InTarget) { Target = InTarget; }
	FORCEINLINE void SetData(FActionData* InData) { Data = InData; }
	void OnTarget(uint32 num);

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SpawnImpactEffect(FVector Location, FRotator Rotation);
private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ImpactParticle;

	AActor* Target;
	FActionData* Data;
};
