#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InPutDataConfig.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SOULLIKE_API UInPutDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UInPutDataConfig();

	UInputMappingContext* InputMappingContext = nullptr;

	UInputAction* Move = nullptr;
	UInputAction* Look = nullptr;
	UInputAction* Jump = nullptr;
	UInputAction* Shift = nullptr;
	UInputAction* Space = nullptr;
	UInputAction* MouseL = nullptr;
	UInputAction* MouseR = nullptr;
	UInputAction* Num1 = nullptr;
	UInputAction* Num2 = nullptr;
	UInputAction* Num3 = nullptr;
	UInputAction* G = nullptr;
	UInputAction* T = nullptr;
	UInputAction* Q = nullptr; 
	UInputAction* E = nullptr; 
	UInputAction* R = nullptr; 
	UInputAction* ESC = nullptr; 
};
