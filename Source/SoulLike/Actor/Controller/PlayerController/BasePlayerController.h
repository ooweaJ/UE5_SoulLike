#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BasePlayerController.generated.h"

UCLASS()
class SOULLIKE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

public:
	void SetPauseMenuOpened(bool bOpen) { bPauseMenuOpened = bOpen; }

	class UConfigMenuWidget* GetConfigMenu() const { return ConfigMenuWidget; }
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLookMouse(const FInputActionValue& InputActionValue);
	void OnJump(const FInputActionValue& InputActionValue);
	void OnEvade(const FInputActionValue& InputActionValue);
	void OnMouseL(const FInputActionValue& InputActionValue);
	void OnMouseR(const FInputActionValue& InputActionValue);
	void OnShift(const FInputActionValue& InputActionValue);
	void OffShift(const FInputActionValue& InputActionValue);
	void OffMouseL(const FInputActionValue& InputActionValue);
	void OffMouseR(const FInputActionValue& InputActionValue);
	void OnQ(const FInputActionValue& InputActionValue); 
	void OnE(const FInputActionValue& InputActionValue); 
	void OnR(const FInputActionValue& InputActionValue); 
	void OnESC(const FInputActionValue& InputActionValue); 
	
private:
	class ABasePlayer* Player;

	TSubclassOf<class UPauseMenuWidget> PauseMenuWidgetClass; 
	TSubclassOf<class UConfigMenuWidget> ConfigMenuWidgetClass; 

	UPROPERTY()
	class UPauseMenuWidget* PauseMenuWidget; 

	UPROPERTY()
	class UConfigMenuWidget* ConfigMenuWidget; 

	bool bPauseMenuOpened = false; 
};
