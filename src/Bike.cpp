#include "Bike.h"

Bike::Bike(BikeWall* wall, ofVec2f startPos, Base4Direction startDir, ofColor color)
{
	Wall = wall;
	Spawn(startPos, startDir);
}

Bike::~Bike()
{
}

void Bike::Update(ofxIlda::Frame* frame)
{
	if (!alive)
		return;
	Position += Speed * Direction;		//Update position
	Wall->ExtendLast(Position);			//Extend last wall segment
	Hitbox.setFromCenter(Position + Direction * Speed * 2, Speed, Speed);	//Update hitbox (to be in front of the bike)
}

void Bike::Spawn(ofVec2f startPos, Base4Direction startDir)
{
	if (alive)
		return;
	Wall->Clear();
	Position = startPos;
	SetDirection(startDir);
	Wall->AddNewLine(startPos);			//We need two new points to have a line to start with.
	Wall->AddNewLine(startPos);
	Hitbox.setFromCenter(Position + Direction * Speed * 2, Speed, Speed);
	alive = true;
}

void Bike::Turn(bool Left)
{
	ofLogNotice("Bike", "Turn %s", ofGetTimestampString().c_str());
	SetDirection(Base4Direction(((int)_Direction + (Left ? 1 : 3)) % 4));		//This is some math magic you need to trust
	Wall->AddNewLine(Position);													//On turns we need a new line to drag with the bike, we leave the previous one behind (hence the corner)
}

void Bike::Respawn(ofVec2f startPos, Base4Direction startDir)
{
	Spawn(startPos, startDir);
}

#ifdef PI
void Bike::TurnCallback(bool left)
{
	ofLogNotice("Bike", "TurnCallback %s", ofGetTimestampString().c_str());
	if(alive)
		Turn(left);
}
#else
void Bike::TurnCallback(int idx)
{
	if (((int)_Direction - idx) % 2 != 0 && alive)		//Some more magic (but trust me it works)
	{
		Turn(_Direction == Base4Direction(idx + 1) % 4);
	}
}
#endif

ofVec2f Bike::getPostition()
{
	return Position;
}

ofRectangle Bike::getHitBox()
{
	return Hitbox;
}

void Bike::Kill()
{
	if(!alive)		//You can't kill what's already dead
		return;
	Wall->Clear();
	alive = false;
	Hitbox = ofRectangle(-1, -1, 0, 0);		//We just push it out of bounds
}

void Bike::SetDirection(Base4Direction d)
{
	_Direction = d;
	switch (d)
	{
	case Bike::Up:
		Direction = ofVec2f(0, -1);
		break;
	case Bike::Down:
		Direction = ofVec2f(0, 1);
		break;
	case Bike::Left:
		Direction = ofVec2f(-1, 0);
		break;
	case Bike::Right:
		Direction = ofVec2f(1, 0);
		break;
	default:
		break;
	}
}
