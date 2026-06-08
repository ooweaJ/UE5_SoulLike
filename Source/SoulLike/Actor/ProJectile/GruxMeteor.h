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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetTarget(AActor* InTarget) { Target = InTarget; }
	FORCEINLINE void SetDamage(float InDamage) { Damage = InDamage; }
	void OnTarget(uint32 num);

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SpawnImpactEffect(FVector Location, FRotator Rotation);

private:
	void ApplyDirectHit(AActor* HitActor);
	void Explode(const FVector& Location, const FRotator& Rotation);

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ImpactParticle;

	UPROPERTY(Replicated)
	AActor* Target;

	UPROPERTY(Replicated)
	float Damage = 20.f;

	bool bExploded = false;
	TArray<TWeakObjectPtr<AActor>> DirectHitActors;
};
