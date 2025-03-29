// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "ASAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

//EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//	
//	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
//	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
//
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
//	if (nullptr == NavSystem) return EBTNodeResult::Failed;
//
//	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
//	FNavLocation NextPatrolPos;
//
//	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.f, NextPatrolPos))
//	{
//		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
//		return EBTNodeResult::Succeeded;
//	}
//	return EBTNodeResult::Failed;
//}
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
		return EBTNodeResult::Failed;

	// 서버에서 미리 지정해준 위치
	FVector PatrolPos = BlackboardComp->GetValueAsVector(BBKEY_PATROLPOS);

	if (PatrolPos.IsNearlyZero())
	{
		// 위치 설정이 안 되어 있음
		return EBTNodeResult::Failed;
	}

	// 이미 위치가 세팅되어 있으므로 Success만 반환
	return EBTNodeResult::Succeeded;
}