// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

#include "ClientPacketHandler.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Managers/SpawnManager.h"

USGameInstance::USGameInstance()
{
}

USGameInstance::~USGameInstance()
{
	if (spawnManager)
	{
		spawnManager->Destroy(); // UObject를 해제하기 위한 호출
		spawnManager = nullptr;
	}
}

void USGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();
		Protocol::C_LOGIN pkt;
		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		SendPacket(SendBuffer);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void USGameInstance::DisconnectFromGameServer()
{
	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void USGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	GameServerSession->HandleRecvPackets();
}

void USGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

TObjectPtr<ASpawnManager> USGameInstance::GetSpawnManager()
{
	
	return spawnManager;
}

