// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ASAIController.h"
#include "BehaviorTree//BehaviorTree.h"
#include "BehaviorTree//BlackboardData.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "ASAI.h"

AASAIController::AASAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_ASCharacter.BB_ASCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_ASCharacter.BT_ASCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AASAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()-> GetActorLocation());
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AASAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent) BTComponent->StopTree();
}

void AASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
