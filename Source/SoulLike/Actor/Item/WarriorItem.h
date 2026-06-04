#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "WarriorItem.generated.h"

UCLASS()
class SOULLIKE_API AWarriorItem : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual void OnDamage(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter);

	virtual void OnDefaultAction();
	virtual void OnDefaultAction2();
	virtual void OnDefaultAction3();
	virtual void OnSkillAction();
	virtual void OnSkillAction2();
	virtual void OnSkillAction3();
	virtual void OnUltimateAction();

	virtual void OffDefaultAction();
	virtual void OffDefaultAction2();
	virtual void OffDefaultAction3();
	virtual void OffSkillAction();
	virtual void OffSkillAction2();
	virtual void OffSkillAction3();
	virtual void OffUltimateAction();

	virtual void OnItemSkillAction();
	virtual void OffItemSkillAction();

	bool Parry;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDefaultDamageType> DDT;
};
