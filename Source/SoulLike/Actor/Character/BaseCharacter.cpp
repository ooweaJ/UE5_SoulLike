#include "Actor/Character/BaseCharacter.h"
#include "Actor/Item/Item.h"
#include "Component/StateComponent.h"
#include "Component/StatusComponent.h"
#include "Component/EquipComponent.h"
#include "Component/MontageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Actor/Item/DamageType/DefaultDamageType.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Equip = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));
	Equip->SetIsReplicated(true);
	State = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	Status->SetIsReplicated(true);
	MontageComponent = CreateDefaultSubobject<UMontageComponent>(TEXT("MontageComponent"));
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultItemClass)
	{
		FTransform DefaultTransform;
		AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(DefaultItemClass, DefaultTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Item->SetOwnerCharacter(this);
		Item->FinishSpawning(DefaultTransform, true);
		Equip->SetSelectItem(Item);
	}

	//SpawnBaseItem();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::EndAction()
{
	if (!State && !Equip) return;

	State->SetIdleMode();
	Equip->EndAction();
}

void ABaseCharacter::HitPlayMontage(TSubclassOf<UDamageType> InDamageType)
{
	if (!InDamageType) return;
	if (UDefaultDamageType* DamageType = Cast<UDefaultDamageType>(InDamageType->GetDefaultObject()))
	{
		if (!MontageComponent) return;
		switch (DamageType->Type)
		{
		case EDamageType::Default:
			break;
		case EDamageType::Hitstop:
			MontageComponent->PlayHit();
			break;
		case EDamageType::Stun:
			MontageComponent->PlayStun();
			break;
		case EDamageType::Knockback:
			MontageComponent->PlayKnockBack();
			break;
		case EDamageType::Knockdown:
			MontageComponent->PlayKnockDown();

			break;
		}
	}
}

void ABaseCharacter::Dead_Implementation()
{
	State->SetDeadMode();
	MontageComponent->PlayDead();
}

void ABaseCharacter::SpawnBaseItem_Implementation()
{
	if (DefaultItemClass)
	{
		FTransform DefaultTransform;
		AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(DefaultItemClass, DefaultTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Item->SetOwnerCharacter(this);
		Item->FinishSpawning(DefaultTransform, true);
		Equip->SetSelectItem(Item);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bRoll) return 0;
	if (State->IsDeadMode()) return 0;

	{
		if(HasAuthority())
		Status->StatusModify(Status->HP, -DamageAmount);

		if (Status->GetCurrentHP() == 0.f)
		{
			Dead();
			return DamageAmount;
		}
	}

	HitPlayMontage(DamageEvent.DamageTypeClass);

	return 0;
}

//void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	if (Equip->GetCurrentItem())
//	{
//		Equip->GetCurrentItem()->EndItem();
//	}
//}


