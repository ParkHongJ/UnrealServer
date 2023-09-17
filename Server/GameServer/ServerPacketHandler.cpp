#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameSessionManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	static std::atomic<uint64> idGenerator = 0;
	static double x = 0;

	if (GameSessionRef gameSession = dynamic_pointer_cast<GameSession>(session))
	{
		shared_ptr<Player> playerRef = make_shared<Player>();
		playerRef->id = idGenerator++;
		playerRef->x = x;
		playerRef->y = 0;
		playerRef->z = 0;
		x += 150;
		
		gameSession->AddPlayer(playerRef);
	}

	SendBufferRef sendbuffer = ServerPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendbuffer);
	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GSessionManager.HandleEnterGame(dynamic_pointer_cast<GameSession>(session), pkt);


	return true;
}

bool Handle_C_SPAWN(PacketSessionRef& session, Protocol::C_SPAWN& pkt)
{
	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
