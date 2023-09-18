// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

#include "ClientPacketHandler.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"

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

		// Protocol::C_LOGIN pkt;
		// SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		// SendPacket(SendBuffer);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void USGameInstance::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	Protocol::C_LEAVE_GAME leavePkt;
	SEND_PACKET(leavePkt);
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

void USGameInstance::SendLoginPacket(FString Id, FString Password)
{
	Protocol::C_LOGIN pkt;
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	SendPacket(SendBuffer);
}

void USGameInstance::HandleSpawn(const Protocol::PlayerInfo& PlayerInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();

	if (World == nullptr)
		return;

	const uint64 ObjectId = PlayerInfo.object_id();
	if(Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());
	AActor* Actor = World->SpawnActor(PlayerClass, &SpawnLocation);

	Players.Add(PlayerInfo.object_id(), Actor);
}

void USGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterPkt)
{
	HandleSpawn(EnterPkt.player());
}

void USGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for(auto& player : SpawnPkt.players())
	{
		HandleSpawn(player);
	}
}

void USGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AActor** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void USGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}
