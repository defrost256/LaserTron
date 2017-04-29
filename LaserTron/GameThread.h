#pragma once

#include "ofThread.h"
#include "EtherDreamController.h"
#include "ofxIldaFrame.h"
#include "GameMap.h"
#include <queue>

using namespace std;

class GameThread : public ofThread
{
public:
	GameThread(EtherDreamController* ed);
	~GameThread();
	
	virtual void threadedFunction() override;

	void QueueInput(int key);
	void Draw();

private:

	ofxIlda::Frame frame;
	queue<int> InputQueue;
	EtherDreamController* ED;

	GameMap* Map;
	vector<Player*> Players;

	void ProcessInputs();
};

