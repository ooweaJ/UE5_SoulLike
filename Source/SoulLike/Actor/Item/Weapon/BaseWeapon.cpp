#include "Actor/Item/Weapon/BaseWeapon.h"
#include "Actor/Item/Attachment.h"
#include "Actor/Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Component/StatusComponent.h"
#include "Component/StateComponent.h"
#include "Net/UnrealNetwork.h"

ABaseWeapon::ABaseWeapon()
{
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(Attachment)
		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &ThisClass::OnDamage);
}

void ABaseWeapon::OnDamage(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	if (!HasAuthority()) return;
	int32 hittedCharactersNum = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	if (hittedCharactersNum == HittedCharacters.Num()) return;

	//Damage
	float LocalDamage = OwnerStatus->GetDamage() + WeaponDamage;
	LocalDamage = LocalDamage * FMath::FRandRange(0.9f, 1.1f);

	FDamageEvent de;
	de.DamageTypeClass = CurrentData.DamageType;
	InOtherCharacter->TakeDamage(LocalDamage, de, InAttacker->GetController(), InCauser);

	//Effect
	UParticleSystem* hitEffect = CurrentData.Effect;
	if (!!hitEffect)
	{
		FTransform transform = CurrentData.EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	if (APlayerController* PC = Cast<APlayerController>(InAttacker->GetController()))
	{
		//Camera Shake
		TSubclassOf<UCameraShakeBase> shake = CurrentData.ShakeClass;
		if (!!shake)
		{
			PC->PlayerCameraManager->StartCameraShake(shake);
		}
	}
}

void ABaseWeapon::OnDefaultAction()
{
	if (bCanCombo == true)
	{
		bCanCombo = false;
		bSucceed = true;

		return;
	}
	if (!OwnerState->IsIdleMode()) return;


	Super::OnDefaultAction();
}

void ABaseWeapon::EndAction()
{
	ComboCount = 1;
	bSucceed = false;
	bCanCombo = false;
}

void ABaseWeapon::ItemAction()
{
	if (!bSucceed) return;
	bSucceed = false;
	OwnerCharacter->StopAnimMontage();
	ComboCount++;
	
	if (FActionData* Data = GetDefaultAction(ComboCount))
	{
		OwnerCharacter->PlayAnimMontage(Data->AnimMontage);
		Data->bCanMove ? OwnerStatus->SetMove() : OwnerStatus->SetStop();
	}
}

void ABaseWeapon::ItemAction2()
{
	if (!bSucceed) return;
	bSucceed = false;
	OwnerCharacter->StopAnimMontage();
	ComboCount++;

	if (FActionData* Data = GetDefaultAction2(ComboCount))
	{
		OwnerCharacter->PlayAnimMontage(Data->AnimMontage);
		Data->bCanMove ? OwnerStatus->SetMove() : OwnerStatus->SetStop();
	}
}

