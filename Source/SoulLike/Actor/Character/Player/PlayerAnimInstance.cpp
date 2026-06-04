#include "Actor/Character/Player/PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	if (!IsValid(character)) return;
	Speed = character->GetVelocity().Length();
	Direction = UKismetAnimationLibrary::CalculateDirection(character->GetVelocity(), character->GetControlRotation());
	Falling = character->GetCharacterMovement()->IsFalling();
}
