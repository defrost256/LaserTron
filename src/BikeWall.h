#pragma once

#include "ofVec2f.h"
#include "ofxIldaFrame.h"

class BikeWall
{
public:
	BikeWall(ofColor c);
	~BikeWall();

	void ExtendLast(ofVec2f point);
	void AddNewLine(ofVec2f point);

	void Update(ofxIlda::Frame* frame, float soundShift);
	void Clear();

	ofVec2f getClosestPointTo(ofVec2f p, float* outDistance = nullptr);
	bool Intersects(ofRectangle shape);

	int BikeIdx;
private:

	ofColor Color;
	ofxIlda::Poly poly;
	int pointCount;
};
