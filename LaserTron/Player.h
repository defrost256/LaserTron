#pragma once

#include "ofColor.h"
#include "ofThread.h"
#include "GameMap.h"
#include "Bike.h"

class GameMap;

class Player : public ofThread
{
public:
	Player(ofColor color, string intputString);
	Player(const Player& p);
	~Player();

	ofColor Color;
	int BikeIdx;
	string InputString;
	Bike* bike;

	GameMap* Map;

	void AddInput(int input);

private:
	int LastInput = -1;

	void ProcessInput();
	void threadedFunction() override;
};