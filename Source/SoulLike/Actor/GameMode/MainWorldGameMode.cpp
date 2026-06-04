#include "Actor/GameMode/MainWorldGameMode.h"
#include "Actor/Controller/PlayerController/BasePlayerController.h"
#include "Actor/PlayerState/BasePlayerState.h"
#include "ASGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Component/StatusComponent.h"

AMainWorldGameMode::AMainWorldGameMode()
{
	PlayerControllerClass = ABasePlayerController::StaticClass();
	{
		ConstructorHelpers::FClassFinder<ABasePlayer> Class = TEXT("/Script/Engine.Blueprint'/Game/_dev/Actor/Character/Player/BP_BasePlayer.BP_BasePlayer_C'");
		ensure(Class.Class);
		DefaultPawnClass = Class.Class;
		PlayerStateClass = ABasePlayerState::StaticClass();
	}
}

void AMainWorldGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

APlayerController* AMainWorldGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	const FString Class = UGameplayStatics::ParseOption(Options, "Class");
	SetPlayerClassName(Class); 
	SpawnRelevantPlayer(PC);

	return PC;
}

void AMainWorldGameMode::SpawnRelevantPlayer(APlayerController* InPlayerController)
{
	TSubclassOf<ABasePlayer>* BasePlayer = ClassMap.Find(ClassName);
	if (!BasePlayer) { return; }

	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (!PlayerStart) { return; }

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InPlayerController;

	ABasePlayer* SpawnedBasePlayer = GetWorld()->SpawnActor<ABasePlayer>(*BasePlayer, SpawnLocation, SpawnRotation, SpawnParams);
	if (!SpawnedBasePlayer) { return; }
	
	AActor* OldPawn = InPlayerController->GetPawn();
	if (ABasePlayer* OldBasePlayer = Cast<ABasePlayer>(OldPawn))
	{
		OldBasePlayer->DestroyAttachedActors(); 
		OldBasePlayer->Destroy(); 
	}
	
	if (ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(InPlayerController))
	{
		BasePlayerController->Possess(SpawnedBasePlayer);
	}

}

void AMainWorldGameMode::Respawn(APlayerController* InPlayerController, float RespawnTime)
{
	FTimerDelegate TimerDelegate; 
	TimerDelegate.BindLambda([this, InPlayerController]()
		{
			RevertToPlayerStart(InPlayerController); 
		});
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, RespawnTime, false);
}

void AMainWorldGameMode::RevertToPlayerStart(APlayerController* InPlayerController)
{
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (!PlayerStart) { return; }

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();

	if (ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(InPlayerController))
	{
		APawn* BasePawn = BasePlayerController->GetPawn(); 
		if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(BasePawn))
		{
			BasePlayer->SetActorLocation(SpawnLocation); 
			BasePlayer->SetActorRotation(SpawnRotation); 
		}
	}
}

void AMainWorldGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainWorldGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
