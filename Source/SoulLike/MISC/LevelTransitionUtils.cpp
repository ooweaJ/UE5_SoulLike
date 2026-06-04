// Fill out your copyright notice in the Description page of Project Settings.
#include "MISC/LevelTransitionUtils.h"
#include "ASGameInstance.h"
#include "Kismet/GameplayStatics.h"


void LevelTransitionUtils::OpenLevelWithData(UObject* WorldContextObject, const FString& LevelName, const FCharacterData& Data)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		if (UASGameInstance* ASGameInstance = Cast<UASGameInstance>(GameInstance))
		{
			FString Option;
			switch (Data.CharacterClassName)
			{
			case ECharacterClass::Warrior:
				Option = "Warrior";
				break;
			case ECharacterClass::Assassin:
				Option = "Assassin";
				break;
			case ECharacterClass::Katana:
				Option = "Katana";
				break;
			case ECharacterClass::Spearman:
				Option = "Spear";
				break;
			}
			FString ClassName(TEXT("Class=") + Option);
			ASGameInstance->SetClassName(ClassName);
			UGameplayStatics::OpenLevel(WorldContextObject, FName(*LevelName)); 
		}
	}

}
