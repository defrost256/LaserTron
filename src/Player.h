#pragma once

#include "ofColor.h"
#include "ofThread.h"
#include "GameMap.h"
#include "Bike.h"
#ifdef PI
#include "wiringPi.h"
#endif

class GameMap;

class Player : public ofThread
{
public:
#ifdef PI
	Player(ofColor color, int leftPin, int rightPin);
#else
	Player(ofColor color, string intputString);
#endif
	Player(const Player& p);
	~Player();

	ofColor Color;
	int BikeIdx;
#ifdef PI
	int LeftPin, RightPin;
#else
	string InputString;
#endif
	Bike* bike;
	GameMap* Map;

#ifdef PI
	void ExecuteInput();
#else
	void AddInput(int input);
#endif

private:
#ifdef PI
	bool leftLast = false, rightLast = false;
	bool hasInput;
#endif
	int LastInput = -1;

	void ProcessInput();
	void threadedFunction() override;
};
