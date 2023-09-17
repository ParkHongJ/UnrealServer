// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStudy.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

class ASpawnManager;
/**
 * 
 */
class FSocket;
UCLASS()
class SERVERSTUDY_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USGameInstance();
	~USGameInstance();
	
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	TObjectPtr<ASpawnManager> GetSpawnManager();

public:
	FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7778;
	TSharedPtr<class PacketSession> GameServerSession;

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASpawnManager> spawnManager; 
};
