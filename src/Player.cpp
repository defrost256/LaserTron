#include "Player.h"

#ifdef PI
Player::Player(ofColor color, int leftPin, int rightPin)
{
	Color = color;
	LeftPin = leftPin;
	RightPin = rightPin;
	WiringPi::pinMode(leftPin, WiringPi::INPUT);
	WiringPi::pinMode(rightPin, WiringPi::INPUT);
//	startThread();
}
#else
Player::Player(ofColor color, string inputString)
{
	Color = color;
	InputString = inputString;
//	startThread();
}
#endif

Player::Player(const Player & p)
{
	Color = p.Color;
	bike = p.bike;
	BikeIdx = p.BikeIdx;
#ifdef PI
	LeftPin = p.LeftPin;
	RightPin = p.RightPin;
#else
	InputString = p.InputString;
#endif
}

Player::~Player()
{
}

#ifdef PI
void Player::ExecuteInput()
{
	lock();
	if(hasInput)
	{
		ofLogNotice("Player", "EXECUTE %s", ofGetTimestampString().c_str());
		if(leftLast)
			bike->TurnCallback(true);
		else if(rightLast)
			bike->TurnCallback(false);
		hasInput = false;
	}
	unlock();
}
#else
void Player::AddInput(int input)
{
	lock();
	LastInput = input;
	unlock();
}
#endif

void Player::ProcessInput()
{
#ifdef PI
	bool leftCurrent, rightCurrent;
	leftCurrent = (WiringPi::digitalRead(LeftPin) == WiringPi::HIGH);
	rightCurrent = (WiringPi::digitalRead(RightPin) == WiringPi::HIGH);
	//ofLogNotice("Player", "%d (%d) <-> %d (%d)", leftCurrent, leftLast, rightCurrent, rightLast);
	//if(leftCurrent != leftLast && rightCurrent != rightLast && leftCurrent && rightCurrent)
	//{
	//	leftLast = leftCurrent;
	//	rightLast = rightCurrent;
	//	Map->RespawnBike(BikeIdx);
	//}
	/*else*/ if(leftCurrent != leftLast)
	{
		if(leftCurrent)
		{
			ofLogNotice("Player", "TURN LEFT %s", ofGetTimestampString().c_str());
			hasInput = true;
		}
		leftLast = leftCurrent;
	}
	else if(rightCurrent != rightLast)
	{
		if(rightCurrent)
		{
			ofLogNotice("Player", "TURN RIGHT %s", ofGetTimestampString().c_str());
			hasInput = true;
		}
		rightLast = rightCurrent;
	}
#else
	if (LastInput != -1)
	{
		if (LastInput < 4)
			bike->TurnCallback(LastInput);
		else
			Map->RespawnBike(BikeIdx);
		LastInput = -1;
	}
#endif
}

void Player::threadedFunction()
{
	while (isThreadRunning())
	{
		lock();
		ProcessInput();
		unlock();
		ofSleepMillis(50);
	}
}
