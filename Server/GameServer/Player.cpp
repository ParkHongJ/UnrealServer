#include "pch.h"
#include "Player.h"
Player::Player(GameSessionRef session)
	: ownerSession(session)
{
	playerInfo = new Protocol::PlayerInfo();	
}

Player::~Player()
{
	delete playerInfo;
}
