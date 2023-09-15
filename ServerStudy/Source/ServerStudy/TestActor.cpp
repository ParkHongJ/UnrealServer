// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "SGameInstance.h"
// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Protocol::C_CHAT msg;
	msg.set_msg("HELLO");
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(msg);
	Cast<USGameInstance>(GetGameInstance())->SendPacket(SendBuffer);
}

