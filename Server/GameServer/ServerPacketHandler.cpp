#include "pch.h"
#include "ServerPacketHandler.h"
#include "ObjectUtils.h"
#include "Room.h"
#include "Player.h"

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

	
	for (int32 i = 0; i < 3; i++)
	{
		Protocol::PlayerInfo* player = loginPkt.add_players();
		player->set_x(Utils::GetRandom(0.f, 100.f));
		player->set_y(Utils::GetRandom(0.f, 100.f));
		player->set_z(Utils::GetRandom(0.f, 100.f));
		player->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	SEND_PACKET(loginPkt);
	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	GRoom->HandleEnterPlayerLocked(player);
	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->ownerRoom.load().lock();
	if (room == nullptr)
		return false;

	room->HandleLeavePlayerLocked(player);
	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
