#include "BikeWall.h"

BikeWall::BikeWall(ofColor c)
{
	PolyCount = 0;
	bDrawLastPoly = false;
}

BikeWall::~BikeWall()
{
}

void BikeWall::ExtendLast(ofVec2f point)
{
	Polys[PolyCount - 1][1].set(point);
	Polys[PolyCount - 1].flagHasChanged();
	bDrawLastPoly = true;
}

void BikeWall::AddNewLine(ofVec2f point)
{
	ofxIlda::Poly newPoly = ofxIlda::Poly(Color);
	newPoly.addVertex(point);
	newPoly.addVertex(point);
	Polys.push_back(newPoly);
	PolyCount++;
	bDrawLastPoly = false;
}

void BikeWall::Update(ofxIlda::Frame* frame)
{
	ofxIlda::Poly tmpPoly;
	if (!bDrawLastPoly)
	{
		tmpPoly = *Polys.end();
		Polys.pop_back();
	}
	frame->addPolys(Polys);
	if (!bDrawLastPoly)
		Polys.push_back(tmpPoly);
}

ofVec2f BikeWall::getClosestPointTo(ofVec2f p, float* outDistance)
{
	ofPoint P = ofPoint(p);
	ofPoint ret = Polys[0].getClosestPoint(P);
	ofPoint tmpP;
	float D = P.distance(ret);
	float tmpD;
	for (int i = 1; i < Polys.size(); i++)
	{
		tmpP = Polys[i].getClosestPoint(P);
		tmpD = P.distance(tmpP);
		if (tmpD < D)
		{
			ret = tmpP;
			D = tmpD;
		}
	}
	if (outDistance != nullptr)
		*outDistance = D;
	return ofVec2f(ret.x, ret.y);
}

bool BikeWall::Intersects(ofRectangle shape)
{
	for (int i = 0; i < PolyCount; i++)
	{
		if (shape.intersects(Polys[i][0], Polys[i][1]))
			return true;
	}
	return false;
}
