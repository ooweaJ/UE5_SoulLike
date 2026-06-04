#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class SOULLIKE_API IMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Host(FString InServerName) = 0;
	virtual void Join(uint32 Index) = 0;
	virtual void LoadMainMenuLevel() = 0;
	virtual void RefreshServerList() = 0;
};
