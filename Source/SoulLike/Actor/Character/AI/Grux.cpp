#include "Actor/Character/AI/Grux.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/ProJectile/GruxMeteor.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Component/StatusComponent.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "UI/InGame/UI_BossStatus.h"
#include "Component/StatusComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

void AGrux::BeginPlay()
{
    Super::BeginPlay();

    TimerDel.BindUFunction(this, FName("SpawnActorsAround"), 500.f, int32(6));
    Tags.Add("Boss");
    NameTag = TEXT("Grux");
}

void AGrux::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority())
    {
        if (bFly)
        {
            FVector location = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 2.f);
            SetActorLocation(location);
        }

        if(bApproach)
        {
            ApproachTarget();
        }

        if(bSkill2)
            MoveToNextPoint();

    }
}

void AGrux::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AGrux, bFly);
    DOREPLIFETIME(AGrux, bTravel);
}

void AGrux::OnFlySkill(FActionData* InData)
{
    Data = InData;
    FVector CurrentLocation = GetActorLocation();

    // 캐릭터의 뒷대각 위 방향 벡터를 계산합니다.
    // 뒤쪽 방향 벡터 (ForwardVector의 반대)
    FVector BackwardVector = -GetActorForwardVector();
    // 위쪽 방향 벡터 (UpVector)
    FVector UpVector = GetActorUpVector();

    // 뒷대각 위 방향 벡터를 계산합니다.
    FVector DiagonalUpwardDirection = (BackwardVector + UpVector).GetSafeNormal();

    // 이동할 위치를 계산합니다.
    TargetLocation = CurrentLocation + DiagonalUpwardDirection * 1500.f;

    bFly = true;

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 3.0f, false);
}

void AGrux::OnSkill2()
{
    bTravel = true;
    bApproach = true;
    Status->SetSpeed(EWalkSpeedTpye::HighRun);
}

void AGrux::SpawnActorsAround(float Distance, int32 NumberOfActors)
{
    FVector Origin = GetActorLocation();
    FRotator Rotation = GetActorRotation();

    for (int32 i = 1; i <= NumberOfActors; ++i)
    {
        float Angle = i * (360.0f / NumberOfActors);
        float Radian = FMath::DegreesToRadians(Angle);

        // Z축을 기준으로 원형 배치
        FVector SpawnLocation = Origin + FVector(0,  FMath::Sin(Radian) * Distance, FMath::Cos(Radian) * Distance);

        // 액터의 회전은 Z축을 기준으로 조정합니다.
        FRotator SpawnRotation = FRotator(0, Angle, 0);

        if (ActorToSpawn)
        {
            if (!BaseController) return;
            AGruxMeteor* Meteor = GetWorld()->SpawnActor<AGruxMeteor>(ActorToSpawn, SpawnLocation, SpawnRotation);
            Meteor->SetTarget(BaseController->GetTarget());
            Meteor->SetOwner(this);
            Meteor->SetData(Data);
            Meteor->OnTarget(i);
        }
    }
    UKismetSystemLibrary::K2_SetTimer(this, "FinishFlySkill", 4.f, false);
}

void AGrux::ApproachTarget()
{
    if (!BaseController) return;
    ACharacter* Target = BaseController->GetTarget();
    if (!Target) return;
    if (300.f > GetDistanceTo(Target))
    {
        bApproach = false;
        UpperSkill();
        return;
    }
    Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction);
}

void AGrux::AirStart()
{
    if (HasAuthority())
    {
        FVector Location = GetActorLocation() + GetActorForwardVector() * 300;
        TArray<FHitResult> HitResults;
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

        if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location, 300.f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true))
        {
            for (const FHitResult& HitResult : HitResults)
            {
                AActor* HitActor = HitResult.GetActor();
                if (HitActor && HitActor->ActorHasTag("Player"))
                {
                    if (ABasePlayer* Player = Cast<ABasePlayer>(HitActor))
                    {
                        if (Player->IsRoll())
                        {
                            EndSkill2();
                            return;
                        }
                        HitPlayer.AddUnique(Player);
                    }
                }
            }

            PlayAirCombo();
        }
    }
}

void AGrux::PlayAirCombo()
{
    if (HitPlayer.Num() > 0)
    {
        for (ABasePlayer* Player : HitPlayer)
        {
            Player->GetCharacterMovement()->GravityScale = 0.5f;
            Player->LaunchCharacter(FVector(0, 0, 1000.f), false, true);
            Player->SetAirbone(true);
            Player->StopAnimMontage();
            FDamageEvent de;
            Player->TakeDamage(50.f, de, GetOwner()->GetInstigatorController(), this);
        }
        UKismetSystemLibrary::K2_SetTimer(this, "StartAirCombo", 1.f, false);
    }
}

void AGrux::AirDamage()
{
    if (HitPlayer.Num() > 0)
    {
        for (ABasePlayer* Player : HitPlayer)
        {
            Player->LaunchCharacter(FVector(0, 0, 100.f), false, true);
            FDamageEvent de;
            Player->TakeDamage(20.f, de, GetOwner()->GetInstigatorController(), this);

            if (HitEffect)
            {
                FTransform transform;
                transform.AddToTranslation(Player->GetActorLocation());
                AirSpawnEffect(transform);
            }
        }
    }
}

void AGrux::StartAirCombo()
{
    bSkill2 = true;
    CenterLocation = GetActorLocation();
    DrawStarPattern(5, 1000.0f);

    UKismetSystemLibrary::K2_SetTimer(this, "EndAirCombo", 5.f, false);
}

void AGrux::EndAirCombo()
{
    if (HitPlayer.Num() > 0)
    {
        for (ABasePlayer* Player : HitPlayer)
        {
            FVector Location = Player->GetActorLocation() - CenterLocation;
            Player->LaunchCharacter(Location * FVector(0,0,-1), false, true);
            FDamageEvent de;
            Player->TakeDamage(50.f, de, GetOwner()->GetInstigatorController(), this);
            Player->GetCharacterMovement()->GravityScale = 1.f;

            if (HitEffect)
            {
                FTransform transform;
                transform.AddToTranslation(Player->GetActorLocation());
                AirSpawnEffect(transform);
            }
        }
    }
    EndSkill2();
}

void AGrux::EndSkill2_Implementation()
{
    HitPlayer.Empty();
    bSkill2 = false;
    bTravel = false;
    bApproach = false;
    EndAction();
}

void AGrux::AirSpawnEffect_Implementation(FTransform InTransform)
{
    if (HitEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, InTransform);
}

void AGrux::DrawStarPattern(int Points, float Radius)
{
    CalculateStarPoints(Points, Radius);
    CurrentPointIndex = 0;
}

void AGrux::CalculateStarPoints(int Points, float Radius)
{
    StarPoints.Empty();

    float AngleStep = 360.0f / Points;
    for (int32 i = 0; i < Points; i++)
    {
        float Angle = UKismetMathLibrary::DegreesToRadians(i * AngleStep * 2);
        float X = CenterLocation.X + Radius * FMath::Cos(Angle);
        float Y = CenterLocation.Y + Radius * FMath::Sin(Angle);
        StarPoints.Add(FVector(X, Y, CenterLocation.Z));
    }
}

void AGrux::MoveToNextPoint()
{
    if (StarPoints.Num() == 0) return;

    FVector TargettoLocation = StarPoints[CurrentPointIndex];
    FVector ToDirection = (TargettoLocation - GetActorLocation()).GetSafeNormal();
    FVector NewLocation = GetActorLocation() + ToDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();

    SetActorLocation(NewLocation);

    if (FVector::Dist(NewLocation, TargettoLocation) < 100.0f)
    {
        AirDamage();
        CurrentPointIndex = (CurrentPointIndex + 1) % StarPoints.Num();
    }
}

void AGrux::UpperSkill_Implementation()
{
    MultiUpperSkill();
}

void AGrux::MultiUpperSkill_Implementation()
{
    PlayAnimMontage(Skill2);
}




void AGrux::FinishFlySkill_Implementation()
{
    MultiFinishFlySkill();
}

void AGrux::MultiFinishFlySkill_Implementation()
{
    if (FlyAttack)
    {
        bFly = false;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        PlayAnimMontage(FlyAttack);
    }
}
