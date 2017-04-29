#include "GameThread.h"

GameThread::GameThread(EtherDreamController * ed)
{
	ED = ed;
	Player* p = new Player(ofColor::aliceBlue, "wasd ");
	Players.push_back(p);
	Map = new GameMap(Players);
	ED->Start();
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
		Map->Update(&frame);
		Map->CheckForCollision();
		ProcessInputs();
		frame.update();
		ED->AddPoints(frame);
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
	lock();
	frame.draw();
	unlock();
}

void GameThread::ProcessInputs()
{
	while (!InputQueue.empty())
	{
		int key = InputQueue.front();
		for (auto it = Players.begin(); it != Players.end(); it++)
		{
			int findIdx = (*it)->InputString.find((char)key);
			if (findIdx < InputQueue.size())
			{
				(*it)->LastInput = findIdx;
			}
		}
	}
}
