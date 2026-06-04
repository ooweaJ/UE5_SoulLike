// Fill out your copyright notice in the Description page of Project Settings.


#include "Others/InteractiveActor.h"
#include "Components/SphereComponent.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Component/EquipComponent.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Sphere->SetSphereRadius(150.f);

	SetRootComponent(DefaultScene);
	Sphere->SetupAttachment(GetRootComponent());
	Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnOverlapEnd);
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::OnInteraction()
{
	UEquipComponent* PlayerEquip = InteractActor->GetEquip();
	PlayerEquip->SupplyPotion();
}

void AInteractiveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(OtherActor))
	{
		InteractActor = BasePlayer;
		InteractActor->InteractableObject = this;
	}
}

void AInteractiveActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(OtherActor))
	{
		if(InteractActor)
			InteractActor->InteractableObject = nullptr;
		InteractActor = nullptr;
	}
}

