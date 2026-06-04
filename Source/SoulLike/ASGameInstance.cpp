#include "ASGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "Engine.h"
#include "UI/UI_ServerMenu.h"
#include "UI/MenuWidget.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Actor/Character/Player/Spearman.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SEVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UASGameInstance::UASGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> mainMenuClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/UI_Session.UI_Session_C'"));
	if (mainMenuClass.Succeeded())
	{
		MainMenuClass = mainMenuClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_Menu.UI_Menu_C'"));
	if (MenuClass.Succeeded())
	{
		InGameMenuClass = MenuClass.Class; 
	}

}

void UASGameInstance::Init()
{
	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (!!oss)
	{
		UE_LOG(LogTemp, Error, TEXT("OSS Pointer Found. Name : %s"), *oss->GetSubsystemName().ToString());

		SessionInterface = oss->GetSessionInterface();


		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);  
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not found OSS"));
	}

	if (!!GEngine)
		GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::OnNetworkFailure);
}

void UASGameInstance::LoadMainMenu()
{
	if (MainMenuClass == nullptr)return;
	MainMenu = CreateWidget<UUI_ServerMenu>(this, MainMenuClass);

	if (MainMenu == nullptr) return;
	MainMenu->SetMenuInterface(this);
	MainMenu->Setup();
}

void UASGameInstance::LoadinGameMenu()
{
	if (!InGameMenuClass) { return; }
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass); 

	if (!InGameMenu) { return; }
	InGameMenu->AddToViewport(); 
	InGameMenu->Setup();
}

void UASGameInstance::Host(FString InServerName)
{
	DesiredServerName = InServerName;

	if (SessionInterface.IsValid())
	{
		auto exsistiongSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (!!exsistiongSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UASGameInstance::Join(uint32 Index)
{
	if (SessionInterface.IsValid() == false) return;
	if (SessionSearch.IsValid() == false) return;

	if (!!MainMenu)
		MainMenu->Teardown();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UASGameInstance::LoadMainMenuLevel()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;
	controller->ClientTravel("/Game/_dev/Level/SessionWorld", ETravelType::TRAVEL_Absolute);
}

void UASGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;  
		SessionSearch->QuerySettings.Set(TEXT("PRESENCESEARCH"), true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Error, TEXT("Start Find Sessions"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UASGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
		SessionInterface->StartSession(SESSION_NAME);
}

void UASGameInstance::SetClassName(FString InName)
{
	ClassName = InName;
}

void UASGameInstance::OnCreateSessionComplete(FName InSessionName, bool InSuccess)
{
	if (InSuccess == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not Create Session"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Session : %s"), *InSessionName.ToString());

	if (!!MainMenu)
		MainMenu->Teardown();

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Host"));

	UWorld* world = GetWorld();
	if (world == nullptr) return;
	UGameplayStatics::OpenLevel(world, FName("/Game/_dev/Level/MainWorld?listen"), true, ClassName);

}

void UASGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	if (InSuccess == true)
		CreateSession();
}

void UASGameInstance::OnFindSessionsComplete(bool InSuccess)
{
	if (InSuccess == true && SessionSearch.IsValid() && MainMenu != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Finished Find Sessions"));

		TArray<FServerData> serverNames;
		for (const FOnlineSessionSearchResult& searchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session ID : %s"), *searchResult.GetSessionIdStr());
			UE_LOG(LogTemp, Warning, TEXT("Ping: %d"), searchResult.PingInMs);

			FServerData data;
			data.MaxPlayers = searchResult.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers = data.MaxPlayers - searchResult.Session.NumOpenPublicConnections;
			data.HostUserName = searchResult.Session.OwningUserName;

			FString serverName;
			if (searchResult.Session.SessionSettings.Get(SEVER_NAME_SETTINGS_KEY, serverName))
			{
				data.Name = serverName;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Session Name no Found"));
			}
			serverNames.Add(data);
		}

		MainMenu->SetServerList(serverNames);
	}
}

void UASGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	if (SessionInterface.IsValid() == false) return;

	FString address;
	if (SessionInterface->GetResolvedConnectString(InSessionName, address) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("not get IP %s"), *address);
		return;
	}

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Join to %s"), *address));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;

	UGameplayStatics::OpenLevel(GetWorld(), FName(address), true, ClassName);
	//controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UASGameInstance::OnNetworkFailure(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InType, const FString& ErrorSting)
{
	LoadMainMenuLevel();

}

void UASGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettiongs;
		//if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		//{
		//	sessionSettiongs.bIsLANMatch = true;
		//}
		//else
		//{
		//	sessionSettiongs.bIsLANMatch = false;
		//}
		sessionSettiongs.bIsLANMatch = true;
		sessionSettiongs.NumPublicConnections = 4;
		sessionSettiongs.bShouldAdvertise = true;
		sessionSettiongs.bUsesPresence = true;
		sessionSettiongs.Set(SEVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettiongs);
	}
}
