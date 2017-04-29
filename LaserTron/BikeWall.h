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

	void Update(ofxIlda::Frame* frame);

	ofVec2f getClosestPointTo(ofVec2f p, float* outDistance = nullptr);
	bool Intersects(ofRectangle shape);
private:

	bool bDrawLastPoly;
	int PolyCount;
	vector<ofxIlda::Poly> Polys;
	ofColor Color;

};

