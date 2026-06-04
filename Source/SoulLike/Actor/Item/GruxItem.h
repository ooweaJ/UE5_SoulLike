#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Weapon/BaseWeapon.h"
#include "GruxItem.generated.h"

UCLASS()
class SOULLIKE_API AGruxItem : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	AGruxItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	virtual void OnDefaultAction();
	virtual void OnDefaultAction2();
	virtual void OnDefaultAction3();
	virtual void OnSkillAction();
	virtual void OnSkillAction2();
	virtual void OnSkillAction3();
	virtual void OnUltimateAction();

	virtual void OffUltimateAction();
	virtual void OnItemSkillAction();
	virtual void ItemAction3();

	class AGrux* Grux;
};
