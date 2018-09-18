#include "BikeWall.h"

BikeWall::BikeWall(ofColor c)
{
	Color = c;
	poly.color = c;
	pointCount = 0;
}

BikeWall::~BikeWall()
{
}

void BikeWall::ExtendLast(ofVec2f point)
{
	pointCount--;
	poly.resize(pointCount);
	AddNewLine(point);
}

void BikeWall::AddNewLine(ofVec2f point)
{
	poly.addVertex(point);
	pointCount++;
}

void BikeWall::Update(ofxIlda::Frame* frame, float soundShift)
{
	float hue, sat, val;
	Color.getHsb(hue, sat, val);
	poly.color = ofColor::fromHsb(hue + soundShift, sat, val);
	frame->addPoly(poly);
}

void BikeWall::Clear()
{
	poly.clear();
	pointCount = 0;
}

ofVec2f BikeWall::getClosestPointTo(ofVec2f p, float* outDistance)
{
	ofPoint ret = poly.getClosestPoint(p);
	if (outDistance != nullptr)
		*outDistance = ret.distance(p);
	return ret;
}

bool BikeWall::Intersects(ofRectangle shape)
{
	for (int i = 0; i < pointCount - 1; i++)
	{
		if (shape.intersects(poly[i], poly[i + 1]))
		{
			return true;
		}
	}
	return false;
}
