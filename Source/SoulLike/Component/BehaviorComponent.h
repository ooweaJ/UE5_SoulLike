#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, patrol, Approach, Melee, Skill, Ultimate, Hitted, KnockBack, Equip, Strafe, Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBehaviorTypeChanged, EBehaviorType, ChangeType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULLIKE_API UBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 현재 블랙보드의 타입이 이 타입이 맞는지 확인 용도
	UFUNCTION(BlueprintPure) bool IsWaitMode();
	UFUNCTION(BlueprintPure) bool IsApproachMode();
	UFUNCTION(BlueprintPure) bool IsMeleeMode();
	UFUNCTION(BlueprintPure) bool IsSkillMode();
	UFUNCTION(BlueprintPure) bool IsUltimateMode();
	UFUNCTION(BlueprintPure) bool IsPatrolMode();
	UFUNCTION(BlueprintPure) bool IsHittedMode();
	UFUNCTION(BlueprintPure) bool IsKnockBackMode();
	UFUNCTION(BlueprintPure) bool IsStrafeMode();


public:
	// 컨트롤러에서 BlackBoard 받아와서 이 컴포넌트에서 관리할 수 있게 해줌
	FORCEINLINE void SetBlackBoard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	// 블랙보드의 타겟과 위치값을 반환해주는 함수
	class ACharacter* GetTarget();
	FVector GetLocation();

	// 블랙보드의 Enum값을 변경하기 위한 함수들
	void SetWaitMode();
	void SetApproachMode();
	void SetMeleeMode();
	void SetSkillMode();
	void SetUltimateMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetKnockBackMode();
	void SetEquipMode();
	void SetStrafeMode();
	void SetDeadMode();

private:
	// 위에 Set 시리즈에서 값을 받아와서 블랙보드 값에 써주는 함수
	void ChangeType(EBehaviorType InType);
	// 현재 타입을 반환하는 함수
	EBehaviorType GetType();

public:
	// Enum 값을 넘기는 용도의 델리게이트 쓴다면 애니메이션 안쓸 확률 높음
	UPROPERTY(BlueprintAssignable)
	FBehaviorTypeChanged OnBehaviorTypeChanged;
	bool bDoOnce;

private:
	class UBlackboardComponent* Blackboard;
};
