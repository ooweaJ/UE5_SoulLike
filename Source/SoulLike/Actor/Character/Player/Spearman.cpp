// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Player/Spearman.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Animation/AnimBlueprint.h"
#include "Actor/Item/Weapon/SpearProjectile.h"
#include "Actor/Item/Weapon/SpearWeapon.h"
#include "Actor/Item/Attachment.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Notifies/AN_ThrowSpear.h"
#include "Components/BoxComponent.h"
#include "Component/StatusComponent.h"
#include "Component/StateComponent.h"
#include "Net/UnrealNetwork.h"

ASpearman::ASpearman()
{
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> ABPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Retargeted_Asset/ABP_Spearman.ABP_Spearman_C'"));
		ensure(ABPClass.Class); 
		GetMesh()->SetAnimInstanceClass(ABPClass.Class); 
	}
}

ASpearWeapon* ASpearman::GetSpearWeapon() const
{
	if (DefaultItemClass && DefaultItemClass->IsChildOf(ASpearWeapon::StaticClass()))
	{
		return DefaultItemClass->GetDefaultObject<ASpearWeapon>();
	}
	else
	{
		check(false); 
		return nullptr; 
	}
}

void ASpearman::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform); 

	if (USkeletalMeshComponent* CharacterMesh = Cast<USkeletalMeshComponent>(GetMesh()))
	{
		const TArray<FSkeletalMaterial>& SkeletalMaterials = CharacterMesh->GetSkinnedAsset()->GetMaterials(); 
		const int32 MaterialNum = SkeletalMaterials.Num(); 
		for (int32 i = 0; i < MaterialNum; ++i)
		{
			UMaterialInstanceDynamic* MID = CharacterMesh->CreateDynamicMaterialInstance(i, SkeletalMaterials[i].MaterialInterface); 
			MID->SetVectorParameterValue(TEXT("Color"), FVector4(0.f, 1.f, 0.f, 1.f));
		}
	}
}

void ASpearman::BeginPlay()
{
	Super::BeginPlay(); 

	SetupSpearProjectile();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); 
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASpearman::OnThrowSpearMontageEnded); 
	}
}

void ASpearman::SetupSpearProjectile()
{
	FActorSpawnParameters SpawnParams; 
	SpawnParams.Owner = this; 
	
	FName SocketName = TEXT("Hand_Spear_R"); 
	const FTransform& SocketTransform = GetMesh()->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_ParentBoneSpace);
	const FVector& SocketLocation = SocketTransform.GetLocation(); 
	
	if (Projecttileclass)
	{
		SpearProjectile = GetWorld()->SpawnActor<ASpearProjectile>(Projecttileclass, SocketLocation, FRotator::ZeroRotator, SpawnParams);
		SpearProjectile->SetActorEnableCollision(false);
	}

	UProjectileMovementComponent* ProjectileMovement = SpearProjectile->FindComponentByClass<UProjectileMovementComponent>();
	if (ProjectileMovement)
	{
		ProjectileMovement->SetActive(false); 
	}

	SpearProjectile->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	SpearProjectile->GetBoxComp()->IgnoreActorWhenMoving(this, true); 
	SpearProjectile->SetComponentsVisibility(false);
}

void ASpearman::HandlePlayerRevival()
{
	Super::HandlePlayerRevival(); 

	SpearProjectile->SetComponentsVisibility(false); 
}

void ASpearman::OnQ()
{
	if (bCanPlaySpearSkillMontage && !bCanThrowSpear)
	{
		Super::OnQ();
	}
	
	bCanPlaySpearSkillMontage = false; 
	bCanThrowSpear = true; 
}

void ASpearman::ChangeBoolAtCooltimeOver(float CoolTime)
{
	bCanThrowSpear = false;

	FTimerDelegate TimerDelegate; 
	TimerDelegate.BindLambda([this]()
		{
			bCanThrowSpear = false; 
			bCanPlaySpearSkillMontage = true; 
		});
	GetWorld()->GetTimerManager().SetTimer(SpearSkillCoolHandle, TimerDelegate, CoolTime, false);
}

void ASpearman::OnThrowSpearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage)
	{
		const TArray<FAnimNotifyEvent>& NotifyEvents = Montage->Notifies; 

		for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
		{
			UAnimNotify* AnimNotify = NotifyEvent.Notify; 
			if (UAN_ThrowSpear* ThrowSpearNotify = Cast<UAN_ThrowSpear>(AnimNotify))
			{
				SpearProjectile->Destroy(); 

				TArray<AActor*> AttachedActors; 
				GetAttachedActors(AttachedActors); 
				for (AActor* AttachedActor : AttachedActors)
				{
					if (AAttachment* SpearAttachment = Cast<AAttachment>(AttachedActor))
					{
						SpearAttachment->SetActorHiddenInGame(false); 
					}
				}

				SetupSpearProjectile(); 
			}
		}
	}

	ChangeBoolAtCooltimeOver(3.f); 
}

void ASpearman::ThrowSpear_Implementation()
{
	SpearProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FName SocketName = TEXT("Hand_Spear_R");

	if (!GetController()) return;
	const FRotator& SpearControlRotation = GetController()->GetControlRotation();
	SpearControlVector = SpearControlRotation.Vector(); 

	SpearProjectile->SetActorRotation(SpearControlRotation - FRotator(0., 90., 0.)); 
	UProjectileMovementComponent* ProjectileComp = SpearProjectile->GetProjectileComp();
	ProjectileComp->Velocity = SpearControlVector * 1500.f;
	ProjectileComp->Activate(true);
	SpearProjectile->SetActorEnableCollision(true);
}

void ASpearman::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpearman, bCanPlaySpearSkillMontage);
	DOREPLIFETIME(ASpearman, bCanThrowSpear);
	DOREPLIFETIME(ASpearman, SpearControlVector);
}
