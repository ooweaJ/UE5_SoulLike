#include "Actor/ProJectile/GruxMeteor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"

namespace
{
	const TCHAR* DefaultMeteorImpactPath = TEXT("/Game/MegaMagicVFXBundle/VFX/MagicAuraVFX/VFX/Meteor/Systems/N_Meteor.N_Meteor");

	UNiagaraSystem* GetDefaultMeteorImpactEffect()
	{
		return LoadObject<UNiagaraSystem>(nullptr, DefaultMeteorImpactPath);
	}
}

AGruxMeteor::AGruxMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	{
		Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		SetRootComponent(Sphere);
		Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	}

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionResponseToAllChannels(ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetGenerateOverlapEvents(true);

	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed = 3000.f;
	Projectile->ProjectileGravityScale = 0.f;
	Projectile->bSweepCollision = true;
	Projectile->bAutoActivate = false;

	InitialLifeSpan = 7.f;
}

void AGruxMeteor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AGruxMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGruxMeteor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGruxMeteor, Target);
	DOREPLIFETIME(AGruxMeteor, Damage);
}

void AGruxMeteor::OnTarget(uint32 num)
{
	if (!HasAuthority()) return;

	float Delay = num * 0.5f;
	UKismetSystemLibrary::K2_SetTimer(this, "Activate", Delay, false);
}

void AGruxMeteor::Activate()
{
	if (!HasAuthority()) return;
	if (!Target) return;

	FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();

	Projectile->Velocity = TargetDirection * 20;
	Projectile->Activate();
}

void AGruxMeteor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) return;
    if (bExploded) return;
    if (OtherActor == GetOwner()) return;
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        return;
    }

    const FVector ImpactPoint = FVector(Hit.ImpactPoint);
    FVector Location = ImpactPoint.IsNearlyZero() ? GetActorLocation() : ImpactPoint;
    FRotator Rotation = Hit.ImpactNormal.IsNearlyZero() ? GetActorForwardVector().Rotation() : Hit.ImpactNormal.Rotation();

    Explode(Location, Rotation);
}

void AGruxMeteor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;
    if (bExploded) return;
    if (!OtherActor || OtherActor == GetOwner()) return;
}

void AGruxMeteor::Explode(const FVector& Location, const FRotator& Rotation)
{
    if (!HasAuthority()) return;
    if (bExploded) return;

    bExploded = true;

    MultiCast_SpawnImpactEffect(Location, Rotation);

    TArray<FHitResult> HitResults;
    TArray<AActor*> DamagedActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location, ExplosionRadius, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true))
    {
        for (const FHitResult& HitResult : HitResults)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor && HitActor->ActorHasTag("Player") && !DamagedActors.Contains(HitActor))
            {
                DamagedActors.Add(HitActor);

                float LocalDamage = Damage * FMath::FRandRange(0.9f, 1.1f);

                FDamageEvent DamageEvent;
                AController* InstigatorController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
                HitActor->TakeDamage(LocalDamage, DamageEvent, InstigatorController, this);

                if (ACharacter* HitCharacter = Cast<ACharacter>(HitActor))
                {
                    FVector KnockbackDirection = HitCharacter->GetActorLocation() - Location;
                    KnockbackDirection.Z = 0.f;

                    if (KnockbackDirection.IsNearlyZero())
                    {
                        KnockbackDirection = Projectile->Velocity;
                        KnockbackDirection.Z = 0.f;
                    }

                    KnockbackDirection = KnockbackDirection.GetSafeNormal();
                    HitCharacter->LaunchCharacter(KnockbackDirection * 450.f + FVector(0.f, 0.f, 220.f), true, true);
                }
            }
        }
    }

    Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Projectile->StopMovementImmediately();
    SetActorHiddenInGame(true);
    SetLifeSpan(1.f);
}

void AGruxMeteor::MultiCast_SpawnImpactEffect_Implementation(FVector Location, FRotator Rotation)
{
	UNiagaraSystem* Effect = ImpactParticle ? ImpactParticle : GetDefaultMeteorImpactEffect();
    if (Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Location, Rotation);
    }
}
