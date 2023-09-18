#include "pch.h"
#include "Room.h"
#include "Player.h"
RoomRef GRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;	

	bool success = EnterPlayer(player);

	player->playerInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_z(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_yaw(Utils::GetRandom(0.f, 100.f));

	//입장 사실을 신입 플레이어한테 알린다
	{
		Protocol::S_ENTER_GAME enterPkt;
		enterPkt.set_success(true);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterPkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);
		if (auto session = player->ownerSession.lock())
		{
			session->Send(sendBuffer);
		}
	}	
	 
	//입장 사실을 다른 플레이어한테 알린다
	{
		Protocol::S_SPAWN spawnPkt;
		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->playerInfo->object_id());
	}

	//기존에 있던 사람들의 정보를 신입 플레이어에게 알린다.
	{
		Protocol::S_SPAWN spawnPkt;
		for (auto& item : _players)
		{
			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->playerInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (GameSessionRef session = player->ownerSession.lock())
			session->Send(sendBuffer);
	}
	return success;
}

bool Room::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->playerInfo->object_id();
	bool success = LeavePlayer(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	{
		Protocol::S_LEAVE_GAME leavePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leavePkt);
		if (GameSessionRef session = player->ownerSession.lock())
			session->Send(sendBuffer);		
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->ownerSession.lock())
			session->Send(sendBuffer);
	}

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		PlayerRef player = item.second;
		if (player->playerInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->ownerSession.lock())
			session->Send(sendBuffer);
	}
}

bool Room::EnterPlayer(PlayerRef player)
{
	if (_players.find(player->playerInfo->object_id()) != _players.end())
		return false;

	_players.insert(make_pair(player->playerInfo->object_id(), player));

	player->ownerRoom.store(shared_from_this());


	return true;
}

bool Room::LeavePlayer(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_players.find(objectId) == _players.end())
		return false;

	PlayerRef player = _players[objectId];
	player->ownerRoom.store(weak_ptr<Room>());

	_players.erase(objectId);
	return true;
}
