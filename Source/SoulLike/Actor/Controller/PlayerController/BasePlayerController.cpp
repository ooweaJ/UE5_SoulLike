#include "Actor/Controller/PlayerController/BasePlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Actor/Character/Player/BasePlayer.h"
#include "Data/Input/InPutDataConfig.h"
#include "BaseGameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UI/PauseMenuWidget.h"
#include "UI/ConfigMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"

ABasePlayerController::ABasePlayerController()
{
	{
		static ConstructorHelpers::FClassFinder<UPauseMenuWidget> PauseMenuWidgetObject(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_PauseMenu.UI_PauseMenu_C'"));
		ensure(PauseMenuWidgetObject.Class);
		PauseMenuWidgetClass = PauseMenuWidgetObject.Class;
	}

	{
		static ConstructorHelpers::FClassFinder<UConfigMenuWidget> ConfigMenuWidgetObject(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_ConfigMenu.UI_ConfigMenu_C'")); 
		ensure(ConfigMenuWidgetObject.Class); 
		ConfigMenuWidgetClass = ConfigMenuWidgetObject.Class; 
	}
	 
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!Subsystem) return;
	const UInPutDataConfig* InPutDataConfig = GetDefault<UInPutDataConfig>();
	Subsystem->AddMappingContext(InPutDataConfig->InputMappingContext, 0);

	if (ABasePlayer* player = Cast<ABasePlayer>(GetPawn()))
		Player = player;

	if (PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass); 
	}

	if (ConfigMenuWidgetClass)
	{
		ConfigMenuWidget = CreateWidget<UConfigMenuWidget>(this, ConfigMenuWidgetClass);
	}

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		const UInPutDataConfig* InPutDataConfig = GetDefault<UInPutDataConfig>();
		{
			EnhancedInputComponent->BindAction(InPutDataConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
			EnhancedInputComponent->BindAction(InPutDataConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::OnLookMouse);
			EnhancedInputComponent->BindAction(InPutDataConfig->Jump, ETriggerEvent::Started, this, &ThisClass::OnJump);
			EnhancedInputComponent->BindAction(InPutDataConfig->Space, ETriggerEvent::Started, this, &ThisClass::OnEvade);
			EnhancedInputComponent->BindAction(InPutDataConfig->MouseL, ETriggerEvent::Started, this, &ThisClass::OnMouseL);
			EnhancedInputComponent->BindAction(InPutDataConfig->MouseR, ETriggerEvent::Started, this, &ThisClass::OnMouseR);
			EnhancedInputComponent->BindAction(InPutDataConfig->MouseL, ETriggerEvent::Completed, this, &ThisClass::OffMouseL);
			EnhancedInputComponent->BindAction(InPutDataConfig->MouseR, ETriggerEvent::Completed, this, &ThisClass::OffMouseR);
			EnhancedInputComponent->BindAction(InPutDataConfig->Q, ETriggerEvent::Started, this, &ThisClass::OnQ);
			EnhancedInputComponent->BindAction(InPutDataConfig->E, ETriggerEvent::Started, this, &ThisClass::OnE);
			EnhancedInputComponent->BindAction(InPutDataConfig->R, ETriggerEvent::Started, this, &ThisClass::OnR);
			EnhancedInputComponent->BindAction(InPutDataConfig->ESC, ETriggerEvent::Started, this, &ThisClass::OnESC);
			EnhancedInputComponent->BindAction(InPutDataConfig->Shift, ETriggerEvent::Triggered, this, &ThisClass::OnShift);
			EnhancedInputComponent->BindAction(InPutDataConfig->Shift, ETriggerEvent::Completed, this, &ThisClass::OffShift);
		}
	}
}

void ABasePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess(); 
}

void ABasePlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	
	ABasePlayer* ControlledPawn = Cast<ABasePlayer>(GetPawn());

	if (!ControlledPawn) { return; }

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotator(0, ControlledPawn->Camera->GetComponentRotation().Yaw,0).Vector();
	const FVector RightDirection = FRotator(0, ControlledPawn->Camera->GetComponentRotation().Yaw+90, 0).Vector();

	ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);

	float AngleDampingSpeed = 1;

	if (Player)
	{
		if (Player->WalkingDirectionAngle < 45 && Player->WalkingDirectionAngle > -45)
		{
			AngleDampingSpeed = 1.0;

		}
		else
		{
			AngleDampingSpeed = 0.75;
		}
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y * AngleDampingSpeed);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X * AngleDampingSpeed);

		Player->ForwardInput = MovementVector.Y;
		Player->RightInput = MovementVector.X;
		Player->MoveDirection = (ForwardDirection * Player->ForwardInput + RightDirection * Player->RightInput).GetSafeNormal();
		Player->WalkingDirectionAngle = UKismetAnimationLibrary::CalculateDirection(Player->MoveDirection, Player->GetActorRotation());
		if (!Player->bLockOn)
		{
			Player->SetActorRotation(UKismetMathLibrary::RLerp(Player->GetActorRotation(), Player->MoveDirection.Rotation(), GWorld->GetDeltaSeconds() * Player->CharacterRotationAlphaLinearValue, true));
		}
	}
}

void ABasePlayerController::OnLookMouse(const FInputActionValue& InputActionValue)
{
	if (!Player)return;
	if (!Player->bLockOn)
	{
		const FVector ActionValue = InputActionValue.Get<FVector>();
		AddYawInput(ActionValue.X);
		AddPitchInput(ActionValue.Y);
	}
}

void ABasePlayerController::OnJump(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->Jump();
}

void ABasePlayerController::OnEvade(const FInputActionValue& InputActionValue)
{
	FVector LastInput = Player->GetLastMovementInputVector();
	if (!Player) return;
	if (LastInput.IsNearlyZero())
		Player->OnStepBack();
	else
		Player->OnEvade();
}

void ABasePlayerController::OnMouseL(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OnMouseL();

}

void ABasePlayerController::OnMouseR(const FInputActionValue& InputActionValue)
{
	if(Player)
		Player->OnMouseR();
}

void ABasePlayerController::OnShift(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OnShift();
}

void ABasePlayerController::OffShift(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OffShift();
}

void ABasePlayerController::OffMouseL(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OffMouseL();
}

void ABasePlayerController::OffMouseR(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OffMouseR();
}

void ABasePlayerController::OnQ(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OnQ(); 
}

void ABasePlayerController::OnE(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->OnInteraction();
}

void ABasePlayerController::OnR(const FInputActionValue& InputActionValue)
{
	if (Player)
		Player->UsePotion();
}

void ABasePlayerController::OnESC(const FInputActionValue& InputActionValue)
{
	if (PauseMenuWidget && PauseMenuWidget->GetPauseMenuWidgetSwitcher()->GetActiveWidgetIndex() == 0)
	{
		if (!bPauseMenuOpened)
		{
			bPauseMenuOpened = true;
			PauseMenuWidget->AddToViewport();
			FInputModeGameAndUI GameAndUI;
			GameAndUI.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
			SetInputMode(GameAndUI); 
			SetShowMouseCursor(true); 
			// UGameplayStatics::SetGamePaused(this, true); 
		}
		else
		{
			bPauseMenuOpened = false;
			PauseMenuWidget->RemoveFromParent(); 
			// UGameplayStatics::SetGamePaused(this, false); 
			SetInputMode(FInputModeGameOnly()); 
			SetShowMouseCursor(false); 
		}
	}
	
}
