#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InGame/UI_ProgressBar.h"
#include "UI_PlayerStatus.generated.h"

UCLASS()
class SOULLIKE_API UUI_PlayerStatus : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UUI_ProgressBar* HP;
	UPROPERTY(meta = (BindWidget))
	UUI_ProgressBar* MP;
	UPROPERTY(meta = (BindWidget))
	UUI_ProgressBar* SP;
};
