#include "Player.h"

Player::Player(ofColor color, string inputString)
{
	Color = color;
	InputString = inputString;
	startThread();
}

Player::Player(const Player & p)
{
	Color = p.Color;
	bike = p.bike;
	BikeIdx = p.BikeIdx;
	InputString = p.InputString;
}

Player::~Player()
{
}

void Player::AddInput(int input)
{
	lock();
	LastInput = input;
	unlock();
}

void Player::ProcessInput()
{
	if (LastInput != -1)
	{
		if (LastInput < 4)
			bike->TurnCallback(LastInput);
		else
			Map->RespawnBike(BikeIdx);
		LastInput = -1;
	}
}

void Player::threadedFunction()
{
	while (isThreadRunning())
	{
		lock();
		ProcessInput();
		unlock();
		ofSleepMillis(10);
	}
}
