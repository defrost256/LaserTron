
#pragma once

#include "Bike.h"
#include "BikeWall.h"
#include "Player.h"
#include "ofxIldaFrame.h"
#include "ofVec2f.h"

class Player;

class GameMap
{
public:
	GameMap(const vector<Player*> &players);
	~GameMap();

	int CheckForCollision();

	void Update(ofxIlda::Frame* frame, float soundShift);
	void RespawnBike(int idx);
	void Reset();

private:

	int GetSpawnPointIdx();
	void KillBike(int idx);

	vector<Bike*> Bikes;
	vector<BikeWall*> Walls;
	vector<Player*> Players;
	ofVec2f SpawnPoints[4] = { ofVec2f(0.05, 0.05), ofVec2f(0.05, 0.95),  ofVec2f(0.95, 0.95), ofVec2f(0.95, 0.05) };

	int LiveBikes = 0;
	ofVec2f Size;
	bool bRequiresManualReset = false;
};

