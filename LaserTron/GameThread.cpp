#include "GameThread.h"

GameThread::GameThread(EtherDreamController * ed)
{
	Player* p = new Player(ofColor::aliceBlue, "wasd ");
	Players.push_back(p);
	Map = new GameMap(Players);
#ifdef LASER
	ED = ed;
	ED->Start();
#endif
	startThread();
}

GameThread::~GameThread()
{
}

void GameThread::threadedFunction()
{
	while (isThreadRunning())
	{
		ofLogNotice("Tron - GameThread", "Frame %d start", Tick);
		lock();
		frame.clear();
		ofLogNotice("Tron - GameThread", "Clear Frame");
		Map->Update(&frame);
		ofLogNotice("Tron - GameThread", "Check collide");
		Map->CheckForCollision();
		ofLogNotice("Tron - GameThread", "Process input");
		ProcessInputs();
		ofLogNotice("Tron - GameThread", "Update frame");
		frame.update();
#ifdef LASER
		ofLogNotice("Tron - GameThread", "Send Points");
		ED->AddPoints(frame);
#endif
		unlock();
		ofLogNotice("Tron - GameThread", "Frame %d end", Tick);
		Tick++;
		ofSleepMillis(50);
	}
}

void GameThread::QueueInput(int key)
{
	lock();
	InputQueue.push(key);
	unlock();
}

void GameThread::Draw()
{
#ifdef DRAW
	lock();
	frame.draw();
	unlock();
#endif
}

void GameThread::ProcessInputs()
{
	while (!InputQueue.empty())
	{
		int key = InputQueue.front();
		for (auto it = Players.begin(); it != Players.end(); it++)
		{
			int findIdx = (*it)->InputString.find((char)key);
			if (findIdx != -1 && findIdx < (*it)->InputString.size())
			{
				(*it)->AddInput(findIdx);
			}
		}
		InputQueue.pop();
	}
}
