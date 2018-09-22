#include "GameThread.h"

GameThread::GameThread(ofxEtherdream * ed)
{
#ifdef PI												//This is the real game logic
	Player* p = new Player(ofColor::green, 2, 3);		//create players
	Players.push_back(p);
	p = new Player(ofColor::red, 4, 14);
	Players.push_back(p);
#else
	Player* p = new Player(ofColor::green, "wasd ");
	Players.push_back(p);
#endif
	Map = new GameMap(Players);							//create map (comment to here for DEBUG)
	for (int i = 0; i < frameBufferSize; i++)			//Create frame buffer
	{
		frames.push_back(ofxIlda::Frame());
#ifdef LASER
		frames[i].params.output.transform.doFlipX = true;	//For laser we need to flip the axes
		frames[i].params.output.transform.doFlipY = true;
#endif
	}
	state = GT_Countdown;								//this is the real game logic
	countdown = 3;										//initialize game state (comment to here for DEBUG)

#ifdef LASER
	ED = ed;											//initialize ether dream
	ED->setup(false, 0);
	ofSleepMillis(1000);								//wait for a second so it's ready for sure
	ED->start();										//start it
#endif

	startThread();										//start the main loop
}

GameThread::~GameThread()
{
}

void GameThread::threadedFunction()
{

	while (isThreadRunning())
	{
		switch(state)									//this is the real game logic
		{
		case GameThread::GT_Stopped:					//if stopped
			ofSleepMillis(100);							//just check back periodically
			break;
		case GameThread::GT_Running:					//if running
			Update();									//update frequently
			ofSleepMillis(15);
			break;
		case GameThread::GT_Countdown:					//if countdown
			Countdown();								//just start the countdown, and wait
			break;
		}												//comment to here for DEBUG
		/*frames[writeBufferIdx].clear();					//This is debug code
		frames[writeBufferIdx].drawCalibration();
		frames[writeBufferIdx].update();
		lock();
		writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
		readBufferIdx = (readBufferIdx + 1) % frameBufferSize;
		unlock();
		ofSleepMillis(10);*/								//DEBUG to here
	}
}

void GameThread::Update()
{
	frames[writeBufferIdx].clear();			//clear the frame we write to
	lock();									//thread safe
	int bikes = Map->CheckForCollision();	//check collision
#ifdef PI
	for(int i = 0; i < Players.size(); i++)	//Check GPIO input
		Players[i]->ExecuteInput();
#else
	ProcessInputs();						//process keyboard events
#endif
	unlock();								//no need for thread safety (we use double buffering)
	Map->Update(&frames[writeBufferIdx], Sound.GetData());														//Update map
	frames[writeBufferIdx].addPoly(ofPolyline::fromRectangle(ofRectangle(0, 0, 1, 1)), ofColor::blue);			//Draw map border
	frames[writeBufferIdx].setPolyProcessorTarget(bikes <= 0 ? targetPoints : targetPoints / (bikes + 1));		//Optimize point count
	frames[writeBufferIdx].update();																			//finalize frame
	Tick++;
	lock();
	readBufferIdx = (readBufferIdx + 1) % frameBufferSize;														//swap buffers
	writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
	unlock();
}

void GameThread::Countdown()
{
	frames[writeBufferIdx].setPolyProcessorTarget(targetPoints);	//optimize points
	DrawFont(countdown > 0 ? countdown + '0' : 'G');				//Draw countdown
	ofSleepMillis(900);												//wait a second
	if(countdown == 0)												//if countdown finished
		state = GT_Running;											//play the game
	countdown--;
}

void GameThread::DrawFont(char c)
{
	frames[writeBufferIdx].clear();						//clear frame we write to
	switch(c)											//draw the number
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
	frames[writeBufferIdx].update();					//finalize the frame
	lock();
	readBufferIdx = (readBufferIdx + 1) % frameBufferSize;		//swap buffers
	writeBufferIdx = (writeBufferIdx + 1) % frameBufferSize;
	unlock();
}

void GameThread::SendData()
{
#ifdef LASER
	lock();
	if (readBufferIdx < 0)	//the readbuffer is uninitialized before the first frame
	{
		unlock();
		return;
	}
	ED->addPoints(frames[readBufferIdx]);	//Send those frames
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
	waitForThread();			//stop game loop
	ED->stop();					//stop etherdream
#ifndef PI
	while(!InputQueue.empty())	//empty input queue
		InputQueue.pop();
#endif
	state = GT_Countdown;		//initialize countdown
	countdown = 3;
	Map->Reset();				//reset the map
	ED->start();				//start etherdream
	startThread();				//start game loop
}

#ifndef PI
void GameThread::ProcessInputs()
{
	while (!InputQueue.empty())
	{
		bool found = false;
		int key = InputQueue.front();												//get the next key in queue
		for (auto it = Players.begin(); it != Players.end() && !found; it++)		//find the player which has that key as input
		{
			int findIdx = (*it)->InputString.find((char)key);
			if (findIdx != -1 && findIdx < (*it)->InputString.size())
			{
				(*it)->AddInput(findIdx);											//add the input
				found = true;
			}
		}
		InputQueue.pop();															//remove from queue
	}
}
#endif

