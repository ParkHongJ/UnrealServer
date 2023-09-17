#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::AddRoom(GameSessionRef session)
{
	WRITE_LOCK;
	_roomPlayer.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
	
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer, GameSessionRef _session)
{
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		if (session != _session)
		{
			session->Send(sendBuffer);
		}
		else
			int a = 10;
	}
}

void GameSessionManager::HandleEnterGame(GameSessionRef session, Protocol::C_ENTER_GAME& pkt)
{
	{
		//본인한테 정보 전송
		Protocol::S_ENTER_GAME enterPkt;
		enterPkt.set_success(true);
		
		auto player = enterPkt.mutable_player();
		player->set_playerid(session->GetPlayer().get()->id);
		player->set_x(session->GetPlayer().get()->x);
		player->set_y(session->GetPlayer().get()->y);
		player->set_z(session->GetPlayer().get()->z);

		SendBufferRef sendbuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);

		session->Send(sendbuffer);
	}
	{
		//본인한테 정보 전송
		//Protocol::S_SPAWN spawnPkt;
		//{
		//	//WRITE_LOCK;
		//	for (auto _session : _sessions)
		//	{
		//		if (_session != session)
		//		{
		//			auto player = spawnPkt.add_players();
		//			player->set_playerid(_session->GetPlayer().get()->id);
		//			player->set_x(_session->GetPlayer().get()->x);
		//			player->set_y(_session->GetPlayer().get()->y);
		//			player->set_z(_session->GetPlayer().get()->z);
		//		}
		//	}
		//}

		//SendBufferRef sendbuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);

		//session->Send(sendbuffer);
	}
	{
		//타인한테 정보 전송
		Protocol::S_SPAWN spawnPkt;
		{
			auto player = spawnPkt.add_players();
			player->set_playerid(session->GetPlayer().get()->id);
			player->set_x(session->GetPlayer().get()->x);
			player->set_y(session->GetPlayer().get()->y);
			player->set_z(session->GetPlayer().get()->z);
		}

		SendBufferRef sendbuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);

		GSessionManager.Broadcast(sendbuffer, session);
	}
}
