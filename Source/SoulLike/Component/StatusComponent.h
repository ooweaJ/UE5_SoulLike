#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

UENUM(BlueprintType)
enum class EWalkSpeedTpye : uint8
{
	Sneak, Walk, Run, HighRun, Max
};

USTRUCT(Atomic, BlueprintType)
struct SOULLIKE_API FStatus
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Regen;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULLIKE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetSneakSpeed() { return Speed[(int32)EWalkSpeedTpye::Sneak]; }
	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)EWalkSpeedTpye::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)EWalkSpeedTpye::Run]; }
	FORCEINLINE float GetMaxHP() { return HP.Max; }
	FORCEINLINE float GetCurrentHP() { return HP.Current; }
	FORCEINLINE float GetDamage() { return Damage; }
	// 캐릭터 이동 제어를 위해 받아옴
	FORCEINLINE bool IsCanMove() { return bCanMove; }
	// 이동제어
	FORCEINLINE void SetMove() { bCanMove = true; }
	FORCEINLINE void SetStop() { bCanMove = false; }
	// 원하는 스피드 값 설정
	UFUNCTION(NetMulticast, Reliable)
	void SetSpeed(EWalkSpeedTpye InType);

	FORCEINLINE float GetEvadeCost() { return EvadeCost; }
	FORCEINLINE float GetAttackCost() { return AttackCost; }

public:

	UFUNCTION(BlueprintCallable)
	void StatusModify(UPARAM(ref) FStatus& Status, float InAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float StatusPersent(const FStatus& Status);

	void StatusRegen(FStatus& Status);
	void SetDamage(float InAmount);
	UFUNCTION()
	void OnRep_Update();
private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)EWalkSpeedTpye::Max] = { 150, 300, 600, 900 };

private:
	float Damage;
	bool bCanMove = true;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Status", ReplicatedUsing = "OnRep_Update")
	FStatus HP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Status")
	FStatus MP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Status")
	FStatus SP;

protected:
	UPROPERTY(EditAnywhere, Category = "Cost")
	float EvadeCost = -50.f;
	UPROPERTY(EditAnywhere, Category = "Cost")
	float AttackCost = -30.f;
};
