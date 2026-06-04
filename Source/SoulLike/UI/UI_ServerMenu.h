// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_SessionWidget.h"
#include "UI_ServerMenu.generated.h"

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class SOULLIKE_API UUI_ServerMenu : public UUI_SessionWidget
{
	GENERATED_BODY()
	
public:
	UUI_ServerMenu(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	void SetServerList(TArray<FServerData> InServerNames);
	void SetSelectedIndex(uint32 Index);


private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitPressed();

private:
	void UpdateChildren();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmJoinMeunButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	/// Host Menu
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmHostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostMenuButton;
private:
	TSubclassOf<class UUserWidget> ServerRowClass;
	TOptional<uint32> SelectedIndex;
};
