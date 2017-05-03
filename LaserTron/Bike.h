#pragma once

#include "ofVec2f.h"
#include "BikeWall.h"
#include "ofxIldaFrame.h"

class Bike
{
public:

	enum Base4Direction { Down, Right, Up, Left };

public:
	Bike(BikeWall* wall, ofVec2f startPos, Base4Direction startDir, ofColor color);
	~Bike();

	void Update(ofxIlda::Frame* frame);
	void Respawn(ofVec2f startPos, Base4Direction startDir);

	void TurnCallback(int idx);

	ofVec2f getPostition();
	ofRectangle getHitBox();

	void Kill();

private:
	
	BikeWall* Wall;
	ofVec2f Position;
	ofVec2f Direction;
	Base4Direction _Direction;
	float Speed = 0.01;

	ofxIlda::Poly Graphics;
	ofxIlda::Poly TransformedGraphics;

	void Turn(bool Left);
	void SetDirection(Base4Direction d);
};

