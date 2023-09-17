#pragma once
#include "Session.h"

class Player
{
public:
	uint64 id;
	double x, y, z;
};
class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void AddPlayer(shared_ptr<Player> player) {
		players.insert(make_pair(player->id, player));
	}
	shared_ptr<Player> GetPlayer();
public:
	weak_ptr<class Room> _room;
private:
	map<uint64, shared_ptr<Player>> players;
};