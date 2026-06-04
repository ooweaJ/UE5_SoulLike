// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_MainInGame.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_MainInGame : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* StatusOverlay;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_PlayerStatusBar* UI_HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_PlayerStatusBar* UI_ManaBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_PlayerStatusBar* UI_StaminaBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_QuickSlot* UI_QuickSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_Text* UI_Text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_BossStatus* BPUI_BossStatus;

protected:
	UPROPERTY(BlueprintReadOnly)
	class ABasePlayer* PlayerPawn;
	class UStatusComponent* Status;
};
