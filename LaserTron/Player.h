#pragma once

#include "ofColor.h"
#include "ofThread.h"

typedef void InputCallback(int idx);

class Player
{
public:
	Player(ofColor color, string intputString);
	Player(const Player& p);
	~Player();

	ofColor Color;
	int BikeIdx;
	string InputString;
	int LastInput = -1;

	InputCallback *Turn;
	InputCallback *Respawn;

	void ProcessInput();
};

