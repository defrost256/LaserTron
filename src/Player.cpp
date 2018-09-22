#include "Player.h"

#ifdef PI
Player::Player(ofColor color, int leftPin, int rightPin)
{
	Color = color;
	LeftPin = leftPin;
	RightPin = rightPin;
	WiringPi::pinMode(leftPin, WiringPi::INPUT);	//Set up pinModes
	WiringPi::pinMode(rightPin, WiringPi::INPUT);
//	startThread();									//We start the playerThread from GameThread
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
	if(hasInput)																//if there is new input
	{
		ofLogNotice("Player", "EXECUTE %s", ofGetTimestampString().c_str());
		if(leftLast)															//check left or right
			bike->TurnCallback(true);
		else if(rightLast)
			bike->TurnCallback(false);
		hasInput = false;				//input was processed
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
	leftCurrent = (WiringPi::digitalRead(LeftPin) == WiringPi::HIGH);			//Check pins
	rightCurrent = (WiringPi::digitalRead(RightPin) == WiringPi::HIGH);
	//ofLogNotice("Player", "%d (%d) <-> %d (%d)", leftCurrent, leftLast, rightCurrent, rightLast);
	//if(leftCurrent != leftLast && rightCurrent != rightLast && leftCurrent && rightCurrent)
	//{
	//	leftLast = leftCurrent;
	//	rightLast = rightCurrent;
	//	Map->RespawnBike(BikeIdx);
	//}
	/*else*/ if(leftCurrent != leftLast)	//if there was change
	{
		if(leftCurrent)			//and the button is pressed
		{
			ofLogNotice("Player", "TURN LEFT %s", ofGetTimestampString().c_str());
			hasInput = true;	//set input state
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
	if (LastInput != -1) //if there was input
	{
		if (LastInput < 4)	//and it's a direction
			bike->TurnCallback(LastInput);	//turn the bike
		else
			Map->RespawnBike(BikeIdx);	//if it's respawn, respawn
		LastInput = -1;		//input was processed
	}
#endif
}

void Player::threadedFunction()
{
	while (isThreadRunning())
	{
		lock();
		ProcessInput();		//process input
		unlock();
		ofSleepMillis(50);	//wait
	}
}
