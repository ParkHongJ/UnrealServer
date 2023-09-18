// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStudy.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
class FSocket;
UCLASS()
class SERVERSTUDY_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	UFUNCTION(BlueprintCallable)
	void SendLoginPacket(FString Id, FString Password);

	UFUNCTION(BlueprintCallable)
	void SendEnterGamePacket();
	
	
	UFUNCTION(BlueprintNativeEvent)
	void BPShowCharacterSelect();
	
	void ShowCharacterSelect(const Protocol::S_LOGIN& pkt);
public:
	void HandleSpawn(const Protocol::PlayerInfo& PlayerInfo);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterPkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);
	
	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	
public:
	FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7778;
	TSharedPtr<class PacketSession> GameServerSession;

public:	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerClass;

	TMap<uint64, AActor*> Players;
};
