#include "Actor/Controller/AIController/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFrameWork/Character.h"

#include "Component/BehaviorComponent.h"
#include "Component/StateComponent.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "MISC/MISC.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseAIController::ABaseAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoard");
	Behavior = CreateDefaultSubobject<UBehaviorComponent>("Behavior");
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = SightRadius + 500;
	Sight->PeripheralVisionAngleDegrees = 360.0f;
	Sight->SetMaxAge(2.f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
	Perception->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::K2_SetTimer(this, "CooldownSkill", 20.f, false);
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerAI = Cast<AAIBaseCharacter>(InPawn);
	if (!OwnerAI) return;
	UBlackboardComponent* BlackboardPtr = Blackboard;
	UseBlackboard(BBAsset, BlackboardPtr);
	Behavior->SetBlackBoard(Blackboard);
	RunBehaviorTree(BTAsset);
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

void ABaseAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	ACharacter* ClosestPlayer = nullptr;
	float ClosestDistance = FLT_MAX;  // 가장 가까운 거리 (초기값을 무한대로 설정)

	for (AActor* actor : actors)
	{
		if (actor->ActorHasTag("Player"))
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(actor);
			UStateComponent* State = PlayerCharacter->FindComponentByClass<UStateComponent>();

			if (State && !State->IsDeadMode())
			{
				// AI 컨트롤러가 제어하는 Pawn과 플레이어 사이의 거리 계산
				float DistanceToPlayer = GetPawn()->GetDistanceTo(PlayerCharacter);

				// 가장 가까운 플레이어 찾기
				if (DistanceToPlayer < ClosestDistance)
				{
					ClosestDistance = DistanceToPlayer;
					ClosestPlayer = PlayerCharacter;
				}
			}
		}
	}

	// 가장 가까운 플레이어를 블랙보드에 설정
	Blackboard->SetValueAsObject(FBlackBoardKeyNameTable::TargetKey, ClosestPlayer);
}

void ABaseAIController::CooldownSkill()
{
	uint32 RandomNum = FMath::RandRange(1, 2);
	Blackboard->SetValueAsInt(FBlackBoardKeyNameTable::RandomKey, RandomNum);
	bSkill = true;
}

void ABaseAIController::SetLoactionKey(FVector InLoaction)
{
	Blackboard->SetValueAsVector(FBlackBoardKeyNameTable::LocationKey, InLoaction);
}

void ABaseAIController::SetTargetKey(ACharacter* InCharacter)
{

	Blackboard->SetValueAsObject(FBlackBoardKeyNameTable::TargetKey, InCharacter);
}

void ABaseAIController::OnMeleeAttack()
{
	if (HasAuthority())
	{
		uint32 num = FMath::RandRange(1, 2);

		if (AAIBaseCharacter* AI = Cast<AAIBaseCharacter>(GetPawn()))
		{
			AI->OnMelee(num);
		}
	}
}

void ABaseAIController::OnSkill(uint32 Num)
{
	bSkill = false;
	UKismetSystemLibrary::K2_SetTimer(this, "CooldownSkill", 30.f, false);
	OwnerAI->OnSkill(Num);
}

void ABaseAIController::OnUltimate()
{
	OwnerAI->OnUltimate();
}

void ABaseAIController::MontagePlayRate()
{
	if (AAIBaseCharacter* AI = Cast<AAIBaseCharacter>(GetPawn()))
	{
		uint32 num = FMath::RandRange(0, 1);
		if (num) return;
		AI->PlayRateMontage();
	}
}

ACharacter* ABaseAIController::GetTarget()
{
	return Behavior->GetTarget();
}
