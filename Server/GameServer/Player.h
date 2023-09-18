#pragma once

class GameSession;
class Room;

class Player : public enable_shared_from_this<Player>
{
public:
	Player(GameSessionRef session);
	virtual ~Player();

public:
	Protocol::PlayerInfo* playerInfo;

public:
	//세션은 절대 바뀌지 않는다
	weak_ptr<GameSession> ownerSession;

public:
	//다른 방으로 이동할 수가 있다
	atomic<weak_ptr<Room>> ownerRoom;
};

