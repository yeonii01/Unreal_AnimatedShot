// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "ASAI.h"
#include "AIController.h"
#include "Interface/ASCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ASCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Character/ASCharacterNonPlayer.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;

	IASCharacterAIInterface* AIPawn = Cast<IASCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) return;

	float DetectRadius = AIPawn->GetAIDetectRange();

	AASCharacterNonPlayer* AIPawnActor = Cast<AASCharacterNonPlayer>(ControllingPawn);
	if (nullptr == AIPawnActor) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);	
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANEL_ASACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);

				AIPawnActor->SetCircleColor(FLinearColor(0.55f, 0.13f, 0.13f));
				return;
			}
		}
	}
	AIPawnActor->SetCircleColor(FLinearColor(0.24f, 0.70f, 0.44f));
}
