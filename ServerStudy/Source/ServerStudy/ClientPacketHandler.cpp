#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"
#include "SGameInstance.h"
#include "Managers/SpawnManager.h"
#include "NetworkActorBase/NetworkActorBase.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if(pkt.success())
	{
		//로그인 성공
		//로그인 정보를 받아서 나의 캐릭터들을 선택한다. 그리고 해당캐릭터를 누르면 C_ENTER_GAME을 보낸다.
	}
	
	// USGameInstance* GameInstance = nullptr;
	// for (TObjectIterator<UGameInstance> It; It; ++It)
	// {
	// 	GameInstance = Cast<USGameInstance>(*It);
	// 	break; // 첫 번째 발견된 게임 인스턴스를 사용
	// }
	// if (GameInstance != nullptr)
	// {
	// 	TObjectPtr<ASpawnManager> spawnManager = GameInstance->GetSpawnManager();
	//
	// 	auto player = pkt.players();
	// 	if (!player.empty())
	// 	{
	// 		spawnManager->SpawnObject(ANetworkActorBase::StaticClass(), player[0].playerid());	
	// 	}		
	// }
	//
	 Protocol::C_ENTER_GAME enterGame;
	 SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGame);
	 session->SendPacket(sendBuffer);

	
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	//생성해야함
	auto player = pkt.player();
	UE_LOG(LogTemp, Log, TEXT("EnterGamePlayer"));
	
	USGameInstance* GameInstance = nullptr;

	for (TObjectIterator<UGameInstance> It; It; ++It)
	{
		GameInstance = Cast<USGameInstance>(*It);
		break; // 첫 번째 발견된 게임 인스턴스를 사용
	}

	if (GameInstance != nullptr)
	{
		TObjectPtr<ASpawnManager> spawnManager = GameInstance->GetSpawnManager();
		FVector3d Pos = FVector3d(player.x(), player.y(), player.z());
		spawnManager->SpawnObject(ANetworkActorBase::StaticClass(), player.playerid(), Pos);
	}
	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	auto players = pkt.players();

	UE_LOG(LogTemp, Log, TEXT("SpawnGamePlayer : %d"), players.size());
	
	USGameInstance* GameInstance = nullptr;

	for (TObjectIterator<UGameInstance> It; It; ++It)
	{
		GameInstance = Cast<USGameInstance>(*It);
		break; // 첫 번째 발견된 게임 인스턴스를 사용
	}
	
	for (auto player : players)
	{
		if (GameInstance != nullptr)
		{
			TObjectPtr<ASpawnManager> spawnManager = GameInstance->GetSpawnManager();
			FVector3d Pos = FVector3d(player.x(), player.y(), player.z());
			spawnManager->SpawnObject(ANetworkActorBase::StaticClass(), player.playerid(), Pos);
		}
	}
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();
	
	return true;
}
