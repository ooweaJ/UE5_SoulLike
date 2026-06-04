#include "UI/UI_ServerMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UI/UI_ServerRow.h"

UUI_ServerMenu::UUI_ServerMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> serverRowClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/UI_ServerRow.UI_ServerRow_C'"));
	if (serverRowClass.Succeeded())
		ServerRowClass = serverRowClass.Class;
}

bool UUI_ServerMenu::Initialize()
{
	bool success = Super::Initialize();
	if (success == false) return false;

	if (JoinButton == nullptr) return false;
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OpenJoinMenu);

	if (QuitButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitPressed);

	if (CancelJoinMenu == nullptr) return false;
	CancelJoinMenu->OnClicked.AddDynamic(this, &ThisClass::OpenMainMenu);

	if (CancelHostMenuButton == nullptr) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &ThisClass::OpenMainMenu);

	if (HostButton == nullptr) return false;
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OpenHostMenu);

	if (ConfirmJoinMeunButton == nullptr) return false;
	ConfirmJoinMeunButton->OnClicked.AddDynamic(this, &ThisClass::JoinServer);

	if (ConfirmHostButton == nullptr) return false;
	ConfirmHostButton->OnClicked.AddDynamic(this, &ThisClass::HostServer);

	return true;
}

void UUI_ServerMenu::SetServerList(TArray<FServerData> InServerNames)
{
	ServerList->ClearChildren();
	uint32 i = 0;
	for (const FServerData& serverData : InServerNames)
	{
		UUI_ServerRow* row = CreateWidget<UUI_ServerRow>(this, ServerRowClass);
		if (row == nullptr) return;

		row->ServerName->SetText(FText::FromString(serverData.Name));
		row->HostUser->SetText(FText::FromString(serverData.HostUserName));
		FString fractionText = FString::Printf(TEXT("%d/%d"), serverData.CurrentPlayers, serverData.MaxPlayers);
		row->Connection->SetText(FText::FromString(fractionText));
		row->Setup(this, i++);

		ServerList->AddChild(row);
	}
}

void UUI_ServerMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;

	UpdateChildren();
}

void UUI_ServerMenu::HostServer()
{
	if (!!MenuInterface)
	{
		FString serverName = ServerHostName->GetText().ToString();
		MenuInterface->Host(serverName);
	}
}

void UUI_ServerMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (JoinMenu == nullptr) return;


	if (MenuInterface != nullptr)
		MenuInterface->RefreshServerList();

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UUI_ServerMenu::OpenMainMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UUI_ServerMenu::OpenHostMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (HostMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UUI_ServerMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && !!MenuInterface)
	{
		UE_LOG(LogTemp, Display, TEXT("%d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("not set"));
	}
}

void UUI_ServerMenu::QuitPressed()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->ConsoleCommand("Quit");
}

void UUI_ServerMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UUI_ServerRow* serverRow = Cast<UUI_ServerRow>(ServerList->GetChildAt(i));
		if (!!serverRow)
		{
			serverRow->bSelected = (SelectedIndex.IsSet()) && (SelectedIndex.GetValue()) == i;
		}
	}
}
