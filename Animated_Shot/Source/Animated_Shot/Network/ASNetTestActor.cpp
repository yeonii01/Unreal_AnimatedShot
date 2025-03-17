// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ASNetTestActor.h"
#include "Protocol.pb.h"
#include "Network/ClientPacketHandler.h"
#include "Network/ASGameInstance.h"

// Sets default values
AASNetTestActor::AASNetTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASNetTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASNetTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Protocol::C_CHAT Msg;
	//Msg.set_msg("hello");
	//SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Msg);
	//Cast<UASGameInstance>(GetGameInstance())->SendPacket(SendBuffer);
}

