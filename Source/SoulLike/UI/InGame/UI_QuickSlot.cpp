// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/UI_QuickSlot.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Component/EquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UUI_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ABasePlayer* BP = Cast<ABasePlayer>(PC->GetPawn()))
		{
			UEquipComponent* EC = BP->GetEquip();
			FPotion* Potion = EC->GetPotion();
			if (Potion)
				PlayerPotion = Potion;
		}
	}
}

void UUI_QuickSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerPotion)
	{
		FText Amount = FText::AsNumber(PlayerPotion->Amount);
		PotionAmount->SetText(Amount);
	}
}
