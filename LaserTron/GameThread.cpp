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
		lock();
		frame.clear();
		Map->Update(&frame);
		Map->CheckForCollision();
		ProcessInputs();
		frame.update();
#ifdef LASER
		ED->AddPoints(frame);
#endif
		unlock();
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
