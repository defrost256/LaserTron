#include "Bike.h"

Bike::Bike(BikeWall* wall, ofVec2f startPos, Base4Direction startDir, ofColor color)
{
	Wall = wall;
	Position = startPos;
	SetDirection(startDir);
	Wall->AddNewLine(startPos);
	Graphics.arc(ofVec2f(0, 0), 0.001, 0.001, 0, 360);
	Graphics.color = color;
}

Bike::~Bike()
{
}

void Bike::Update(ofxIlda::Frame* frame)
{
	Position += Speed * Direction;
	Wall->ExtendLast(Position);
	//frame->addPoly(Graphics.getTranslated(ofVec3f(Position)));
}

void Bike::Turn(bool Left)
{
	SetDirection(Base4Direction(((int)_Direction + (Left ? 1 : 3)) % 4));
	Wall->AddNewLine(Position);
}

void Bike::Respawn(ofVec2f startPos, Base4Direction startDir)
{
	Position = startPos;
	SetDirection(startDir);
	Wall->AddNewLine(startPos);
}

void Bike::TurnCallback(int idx)
{
	if (((int)_Direction - idx) % 2 != 0)
	{
		Turn(_Direction == Base4Direction(idx + 1) % 4);
	}
}

ofVec2f Bike::getPostition()
{
	return Position;
}

ofRectangle Bike::getHitBox()
{
	return TransformedGraphics.getBoundingBox();
}

void Bike::Kill()
{
	ofLogNotice("Tron", "Kill");
}

void Bike::SetDirection(Base4Direction d)
{
	_Direction = d;
	switch (d)
	{
	case Bike::Up:
		ofLogNotice("Tron", "Turn UP");
		Direction = ofVec2f(0, -1);
		break;
	case Bike::Down:
		ofLogNotice("Tron", "Turn DOWN");
		Direction = ofVec2f(0, 1);
		break;
	case Bike::Left:
		ofLogNotice("Tron", "Turn LEFT");
		Direction = ofVec2f(-1, 0);
		break;
	case Bike::Right:
		ofLogNotice("Tron", "Turn RIGHT");
		Direction = ofVec2f(1, 0);
		break;
	default:
		break;
	}
}
