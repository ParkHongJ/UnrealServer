#pragma once

class GameSession;

using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void AddRoom(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);
	void Broadcast(SendBufferRef sendBuffer, GameSessionRef _session);

	void HandleEnterGame(GameSessionRef session, Protocol::C_ENTER_GAME& pkt);
private:
	USE_LOCK;
	set<GameSessionRef> _sessions;
	set<GameSessionRef> _roomPlayer;
};

extern GameSessionManager GSessionManager;
