// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/UI_MainInGame.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "UI/InGame/UI_BossStatus.h"
#include "UI/InGame/UI_PlayerStatusBar.h"
#include "UI/InGame/UI_QuickSlot.h"
#include "UI/UI_Text.h"
#include "Component/StatusComponent.h"

void UUI_MainInGame::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	BPUI_BossStatus->SetVisibility(ESlateVisibility::Collapsed);
	UI_Text->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_MainInGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABasePlayer* BP = Cast<ABasePlayer>(GetOwningPlayerPawn()))
	{
		PlayerPawn = BP;
		Status = PlayerPawn->GetStatus();
	}
}

void UUI_MainInGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Status)
	{
		UI_HealthBar->Percent = Status->StatusPersent(Status->HP);
		UI_HealthBar->InWidthOverride = Status->HP.Max;

		UI_ManaBar->Percent = Status->StatusPersent(Status->MP);
		UI_ManaBar->InWidthOverride = Status->MP.Max;

		UI_StaminaBar->Percent = Status->StatusPersent(Status->SP);
		UI_StaminaBar->InWidthOverride = Status->SP.Max;
	}
}
