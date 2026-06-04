#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "MainWorldGameMode.generated.h"

UCLASS()
class SOULLIKE_API AMainWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainWorldGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	void SpawnRelevantPlayer(APlayerController* InPlayerController); 
	void Respawn(APlayerController* InPlayerController, float RespawnTime);
	void RevertToPlayerStart(APlayerController* InPlayerController);

public:
	void SetPlayerClassName(FString InClassName) { ClassName = InClassName; }
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	uint32 NumberOfPlayers = 0;
	FString ClassName;

	UPROPERTY(EditAnywhere)
	TMap<FString, TSubclassOf<ABasePlayer>> ClassMap;

	FTimerHandle RespawnTimerHandle; 
};
