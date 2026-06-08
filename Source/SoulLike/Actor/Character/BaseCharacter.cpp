#include "Actor/Character/BaseCharacter.h"
#include "Actor/Item/Item.h"
#include "Component/StateComponent.h"
#include "Component/StatusComponent.h"
#include "Component/EquipComponent.h"
#include "Component/MontageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Equip = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));
	Equip->SetIsReplicated(true);
	State = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	State->SetIsReplicated(true);
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	Status->SetIsReplicated(true);
	MontageComponent = CreateDefaultSubobject<UMontageComponent>(TEXT("MontageComponent"));
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnBaseItem();
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

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Parrying);
	DOREPLIFETIME(ABaseCharacter, bRoll);
}

void ABaseCharacter::EndAction()
{
	if (!State && !Equip) return;

	State->SetIdleMode();
	Equip->EndAction();
}

void ABaseCharacter::MulticastPlayHitReaction_Implementation(TSubclassOf<UDamageType> InDamageType)
{
	if (MontageComponent)
	{
		MontageComponent->PlayHitReaction(InDamageType);
	}
}

void ABaseCharacter::Dead_Implementation()
{
	State->SetDeadMode();
	MontageComponent->PlayDead();
}

void ABaseCharacter::SpawnBaseItem_Implementation()
{
	if (!HasAuthority()) return;
	if (!DefaultItemClass) return;
	if (Equip && Equip->GetCurrentItem()) return;

	FTransform DefaultTransform;
	AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(DefaultItemClass, DefaultTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Item->SetOwnerCharacter(this);
	Item->FinishSpawning(DefaultTransform, true);
	Equip->SetSelectItem(Item);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HasAuthority()) return 0;
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

	MulticastPlayHitReaction(DamageEvent.DamageTypeClass);

	return 0;
}

//void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	if (Equip->GetCurrentItem())
//	{
//		Equip->GetCurrentItem()->EndItem();
//	}
//}


