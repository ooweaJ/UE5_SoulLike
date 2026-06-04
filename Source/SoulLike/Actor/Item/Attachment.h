#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class SOULLIKE_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();
	void SetOwnerCharacter(ACharacter* InOwnerCharacter);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// 블루프린트에서 무기의 외형을 담당할 클래스로 Equip 함수들을 이용해 메쉬를 숨기거나 보이게한다
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();
	// 무기의 콜리전이 여러개의 경우 소켓위치에 콜리전을 이동시키는 함수
	UFUNCTION(BlueprintCallable)
	void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);

private:
	// 충돌 바인딩 함수
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 노티파이스테이트로 무기의 콜리전을 껏다 키기 위한 함수 ( 여러개의 경우 콜리전의 이름을 받아 특정 부분만 켜줄 수 있음)
	void OnCollisions(FString InCollisionName = "None");
	void OffCollisions();

public:
	// 이 클래스의 충돌 정보를 Item 클래스에게 넘겨주기 위한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
	FAttachmentEndOverlap OnAttachmentEndOverlap;

private:
	TArray<class UShapeComponent*> ShapeComponents;
	USceneComponent* Scene;
	class ACharacter* OwnerCharacter;
	TArray<AActor*> Targets;
};
