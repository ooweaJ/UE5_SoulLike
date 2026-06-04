#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ProgressBar.generated.h"

UCLASS()
class SOULLIKE_API UUI_ProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetPersent(float InPersent) { Persent = InPersent; }

	UPROPERTY(BlueprintReadOnly)
	float Persent;
};
