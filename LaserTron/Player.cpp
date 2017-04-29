#include "Player.h"

Player::Player(ofColor color, string intputString)
{
	Color = color;
	InputString = InputString;
}

Player::Player(const Player & p)
{
	Color = p.Color;
	Turn = p.Turn;
	Respawn = p.Respawn;
	BikeIdx = p.BikeIdx;
	InputString = p.InputString;
}

Player::~Player()
{
}

void Player::ProcessInput()
{
	if (LastInput != -1)
	{
		if (LastInput < 4)
			Turn(LastInput);
		else
			Respawn(BikeIdx);
		LastInput = -1;
	}
}
