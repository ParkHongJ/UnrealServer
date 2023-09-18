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
	//������ ���� �ٲ��� �ʴ´�
	weak_ptr<GameSession> ownerSession;

public:
	//�ٸ� ������ �̵��� ���� �ִ�
	atomic<weak_ptr<Room>> ownerRoom;
};

