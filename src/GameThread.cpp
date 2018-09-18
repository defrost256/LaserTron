#include "GameThread.h"

GameThread::GameThread(ofxEtherdream * ed)
{
/*#ifdef PI
	Player* p = new Player(ofColor::green, 2, 3);
	Players.push_back(p);
	p = new Player(ofColor::red, 4, 14);
	Players.push_back(p);
#else
	Player* p = new Player(ofColor::green, "wasd ");
	Players.push_back(p);
#endif
	Map = new GameMap(Players);
	*/for (int i = 0; i < frameBufferSize; i++)
	{
		frames.push_back(ofxIlda::Frame());
#ifdef LASER
		frames[i].params.output.transform.doFlipX = true;
		frames[i].params.output.transform.doFlipY = true;
#endif
	}/*
	state = GT_Countdown;
	countdown = 3;
	*/
#ifdef LASER
	ED = ed;
	ED->setup(false, 0);
	ofSleepMillis(1000);
	ED->start();
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
	/*	switch(state)
		{
		case GameThread::GT_Stopped:
			ofSleepMillis(100);
			break;
		case GameThread::GT_Running:
			Update();
			ofSleepMillis(15);
			break;
		case GameThread::GT_Countdown:
			Countdown();
			break;
		}*/
		frames[writeBufferIdx].clear();
		frames[writeBufferIdx].drawCalibration();
		frames[writeBufferIdx].update();
		lock();
		writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
		readBufferIdx = (readBufferIdx + 1) % frameBufferSize;
		unlock();
		ofSleepMillis(10);
	}
}

void GameThread::Update()
{
	frames[writeBufferIdx].clear();
	lock();
	int bikes = Map->CheckForCollision();
#ifdef PI
	for(int i = 0; i < Players.size(); i++)
		Players[i]->ExecuteInput();
#else
	ProcessInputs();
#endif
	unlock();
	Map->Update(&frames[writeBufferIdx], Sound.GetData());
	frames[writeBufferIdx].addPoly(ofPolyline::fromRectangle(ofRectangle(0, 0, 1, 1)), ofColor::blue);
	frames[writeBufferIdx].setPolyProcessorTarget(bikes <= 0 ? targetPoints : targetPoints / (bikes + 1));
	frames[writeBufferIdx].update();
	Tick++;
	lock();
	readBufferIdx = (readBufferIdx + 1) % frameBufferSize;
	writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
	unlock();
}

void GameThread::Countdown()
{
	frames[writeBufferIdx].setPolyProcessorTarget(targetPoints);
	DrawFont(countdown > 0 ? countdown + '0' : 'G');
	ofSleepMillis(900);
	if(countdown == 0)
		state = GT_Running;
	countdown--;
}

void GameThread::DrawFont(char c)
{
	frames[writeBufferIdx].clear();
	switch(c)
	{
	case '1':
		frames[writeBufferIdx].addPoly(cdp.pFont_1_0);
		frames[writeBufferIdx].addPoly(cdp.pFont_1_1);
		break;
	case '2':
		frames[writeBufferIdx].addPoly(cdp.pFont_2_0);
		frames[writeBufferIdx].addPoly(cdp.pFont_2_1);
		break;
	case '3':
		frames[writeBufferIdx].addPoly(cdp.pFont_3_0);
		frames[writeBufferIdx].addPoly(cdp.pFont_3_1);
		break;
	case 'G':
		frames[writeBufferIdx].addPoly(cdp.pFont_G_0);
		frames[writeBufferIdx].addPoly(cdp.pFont_G_1);
		frames[writeBufferIdx].addPoly(cdp.pFont_O_0);
		frames[writeBufferIdx].addPoly(cdp.pFont_O_1);
		break;
	}
	frames[writeBufferIdx].update();
	lock();
	readBufferIdx = (readBufferIdx + 1) % frameBufferSize;
	writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
	unlock();
}

void GameThread::SendData()
{
#ifdef LASER
	lock();
	if (readBufferIdx < 0)
	{
		unlock();
		return;
	}
	ED->addPoints(frames[readBufferIdx]);
	unlock();
#endif // LASER
}

#ifndef PI
void GameThread::QueueInput(int key)
{
	lock();
	InputQueue.push(key);
	unlock();
}
#endif

void GameThread::Draw()
{
#ifdef DRAW
	lock();
	if (readBufferIdx < 0)
	{
		unlock();
		return;
	}
	frames[readBufferIdx].draw();
	unlock();
#endif
}

void GameThread::Restart()
{
	waitForThread();
	ED->stop();
#ifndef PI
	while(!InputQueue.empty())
		InputQueue.pop();
#endif
	state = GT_Countdown;
	countdown = 3;
	Map->Reset();
	ED->start();
	startThread();
}

#ifndef PI
void GameThread::ProcessInputs()
{
	while (!InputQueue.empty())
	{
		bool found = false;
		int key = InputQueue.front();
		for (auto it = Players.begin(); it != Players.end() && !found; it++)
		{
			int findIdx = (*it)->InputString.find((char)key);
			if (findIdx != -1 && findIdx < (*it)->InputString.size())
			{
				(*it)->AddInput(findIdx);
				found = true;
			}
		}
		InputQueue.pop();
	}
}
#endif

