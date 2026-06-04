// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Weapon/SpearProjectile.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpearProjectile::ASpearProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	// Box Collision
	{
		Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
		SetRootComponent(Box);
		Box->SetBoxExtent(FVector(180., 430., 180.)); 

		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}

	// Skeletal Mesh
	{
		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh")); 
		SkeletalMesh->SetupAttachment(GetRootComponent()); 

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Boss/Boss_Asset/InfinityBladeWeapons/Weapons/Spear/source/SpearofAdun.SpearofAdun'"));
		ensure(SkeletalAsset.Object); 
		SkeletalMesh->SetSkeletalMesh(SkeletalAsset.Object); 
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Niagara System Component - Aura
	{
		NiagaraAura = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraAura")); 
		NiagaraAura->SetupAttachment(SkeletalMesh); 

		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAuraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/_dev/Effect/Spear/Niagara/NS_SparkAura.NS_SparkAura'"));
		ensure(NiagaraAuraAsset.Object); 
		NiagaraAura->SetAsset(NiagaraAuraAsset.Object); 
	}

	// Niagara System Component - Spark
	{
		NiagaraSpark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSpark"));
		NiagaraSpark->SetupAttachment(SkeletalMesh);

		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSparkAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/_dev/Effect/Spear/Niagara/NS_Electricity.NS_Electricity'"));
		ensure(NiagaraSparkAsset.Object);
		NiagaraSpark->SetAsset(NiagaraSparkAsset.Object);
	}

	// Projectile Movement Component
	{
		ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileComp->InitialSpeed = 3000.f;
		ProjectileComp->ProjectileGravityScale = 0.f; 
	}

}

void ASpearProjectile::SetComponentsVisibility(bool bVisible)
{
	TArray<UActorComponent*> Components; 
	GetComponents(Components); 

	for (UActorComponent* Component : Components)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			PrimitiveComponent->SetVisibility(bVisible, true); 
		}
	}
}

void ASpearProjectile::BeginPlay()
{
	Super::BeginPlay(); 

	Box->OnComponentHit.AddDynamic(this, &ASpearProjectile::OnHit);
	SetComponentsVisibility(false);
}

void ASpearProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		AAIBaseCharacter* BossCharacter = Cast<AAIBaseCharacter>(OtherActor); 

		if (!BossCharacter) { return;  }

		UGameplayStatics::ApplyDamage(BossCharacter, SpearThrowingDamage, nullptr, this, UDamageType::StaticClass()); 
		SetComponentsVisibility(false);

	}
}