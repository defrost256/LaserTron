#include "GameMap.h"

GameMap::GameMap(const vector<Player*> &players)
{
	Players = vector<Player*>(players.begin(), players.end());
	for (auto it = Players.begin(); it != Players.end(); it++)
	{
		(*it)->Map = this;
		BikeWall *wall = new BikeWall((*it)->Color);
		int SpawnIdx = GetSpawnPointIdx();
		Bike *bike = new Bike(wall, SpawnPoints[SpawnIdx], Bike::Base4Direction((SpawnIdx + 1) % 4), (*it)->Color);
		(*it)->BikeIdx = Bikes.size();
		(*it)->bike = bike;
		Walls.push_back(wall);
		Bikes.push_back(bike);
	}
}

GameMap::~GameMap()
{
}

void GameMap::CheckForCollision()
{
	for (int i = 0; i < Walls.size(); i++)
	{
		for (int j = 0; j < Bikes.size(); j++)
		{
			ofRectangle hitBox = Bikes[j]->getHitBox();
			if (Walls[i]->Intersects(hitBox))
				KillBike(j);
		}
	}
	for (int i = 0; i < Bikes.size(); i++)
	{
		ofRectangle hitBox = Bikes[i]->getHitBox();
		if (hitBox.intersects(ofVec2f(0, 0), ofVec2f(0, Size.y)) ||
			hitBox.intersects(ofVec2f(0, 0), ofVec2f(Size.x, 0)) ||
			hitBox.intersects(ofVec2f(0, Size.y), ofVec2f(Size.x, Size.y)) ||
			hitBox.intersects(ofVec2f(Size.x, 0), ofVec2f(Size.x, Size.y)))
		{
			KillBike(i);
		}
		for (int j = i + 1; j < Bikes.size(); j++)
		{
			if (Bikes[j]->getHitBox().intersects(hitBox))
			{
				KillBike(i);
				KillBike(j);
			}
		}
	}
}

void GameMap::Update(ofxIlda::Frame * frame)
{
	for (int i = 0; i < Bikes.size(); i++)
	{
		Bikes[i]->Update(frame);
	}
	for (int i = 0; i < Walls.size(); i++)
	{
		Walls[i]->Update(frame);
	}
}

void GameMap::RespawnBike(int idx)
{
	int SpawnIdx = GetSpawnPointIdx();
	Bikes[idx]->Respawn(SpawnPoints[SpawnIdx], Bike::Base4Direction((SpawnIdx + 1) % 4));
	LiveBikes++;
}

int GameMap::GetSpawnPointIdx()
{
	if (Bikes.size() == 0 || LiveBikes == 0)
		return rand() % 4;
	int furthestIdx = 0;
	float biggestDst = 0;
	float currentDst = 0;
	for (int i = 0; i < 4; i++)
	{
		currentDst = 0;
		for (auto it = Bikes.begin(); it != Bikes.end(); it++)
		{
			currentDst += (*it)->getPostition().distance(SpawnPoints[i]);
		}
		if (currentDst > biggestDst)
		{
			biggestDst = currentDst;
			furthestIdx = i;
		}
	}
	return furthestIdx;
}

void GameMap::KillBike(int idx)
{
	Bikes[idx]->Kill();
	LiveBikes--;
}
