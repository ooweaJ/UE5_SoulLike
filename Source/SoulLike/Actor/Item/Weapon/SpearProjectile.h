// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpearProjectile.generated.h"

class UBoxComponent;
class USkeletalMeshComponent; 
class UNiagaraComponent;
class UNiagaraSystem; 
class UProjectileMovementComponent;


UCLASS()
class SOULLIKE_API ASpearProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpearProjectile();

public:
	void SetComponentsVisibility(bool bVisible);

	UBoxComponent* GetBoxComp() const { return Box; }
	UProjectileMovementComponent* GetProjectileComp() const { return ProjectileComp; }

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY()
	float SpearThrowingDamage = 100.f; 
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* Box; 

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh; 

	UPROPERTY()
	UNiagaraComponent* NiagaraAura; 

	UPROPERTY()
	UNiagaraComponent* NiagaraSpark;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileComp;
};
