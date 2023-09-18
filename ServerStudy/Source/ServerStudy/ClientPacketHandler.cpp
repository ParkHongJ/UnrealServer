#include "ClientPacketHandler.h"
#include "ServerStudy.h"
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
	for (int32 i = 0; i < pkt.players_size(); ++i)
	{
		const Protocol::PlayerInfo& Player = pkt.players(i);
	}
	if (auto* GameInstance = Cast<USGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->ShowCharacterSelect(pkt);
	}
	
	// Protocol::C_ENTER_GAME enterGame;
	//
	// enterGame.set_playerindex(0);
	// SEND_PACKET(enterGame);
	
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	//생성해야함

	if (auto* GameInstance = Cast<USGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt.player());
	}
	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<USGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}
	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<USGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}
	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();
	
	return true;
}
