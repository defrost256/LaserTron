#pragma once

#include "ofThread.h"
#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"
#include "CountdownPoints.h"
#include "GameMap.h"
#include "SoundThread.h"
#include <queue>

using namespace std;

class GameThread : public ofThread
{
public:
	GameThread(ofxEtherdream* ed);
	~GameThread();

	virtual void threadedFunction() override;

	void SendData();

#ifndef PI
	void QueueInput(int key);
#endif
	void Draw();
	
	void Restart();
private:

	enum GT_State{ GT_Stopped, GT_Running, GT_Countdown };

	long Tick = 0;

	vector<ofxIlda::Frame> frames;
	int frameBufferSize = 2;
	int writeBufferIdx = 0, readBufferIdx = -1;
#ifdef PI
	int targetPoints = 500;
#else
	queue<int> InputQueue;
#endif
	ofxEtherdream* ED;
	SoundThread Sound;

	GameMap* Map;
	vector<Player*> Players;
	GT_State state;
	int countdown;
	CountdownPoints cdp;

#ifndef PI
	int SendCount;
	void ProcessInputs();
#endif
	void Update();
	void Countdown();
	void DrawFont(char c);
};
