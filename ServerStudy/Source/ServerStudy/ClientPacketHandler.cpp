#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"
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
	

	auto player = pkt.players();
	if (!player.empty())
	{
		FWorldContext* world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);

		UWorld* uWorld = world->World();
		ANetworkActorBase* actor = Cast<ANetworkActorBase>(uWorld->SpawnActor(ANetworkActorBase::StaticClass()));
		actor->SetID(player[0].playerid());	
	}	
	
	Protocol::C_ENTER_GAME enterGame;
	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGame);
	session->SendPacket(sendBuffer);
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	//생성해야함
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();
	
	return true;
}
