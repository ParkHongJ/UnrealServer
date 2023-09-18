#pragma once
class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	USE_LOCK;
	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);
private:	
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);

private:
	unordered_map<uint64, PlayerRef> _players;
};

extern RoomRef GRoom;

