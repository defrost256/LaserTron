#include "EtherDreamController.h"

EtherDreamController::EtherDreamController(EtherDream * device, bool StartThread)
{
	this->device = device;
	if (device->Connect() < 0)
		ofLogError("EtherDream", "Couldn't create etherdream controller");
	else
	{
		ofLogNotice("EtherDream", "Connected to etherdream device");
		isFound = true;
	}

	if (StartThread)
		startThread(false);
}

EtherDreamController::~EtherDreamController()
{
}

void EtherDreamController::threadedFunction()
{
	while (isThreadRunning())
	{
		if (isFound)
		{
			//ofLogNotice("Loop", "AQUIRING LOCK");
			while (!lock())
				ofSleepMillis(5);
			//ofLogNotice("Loop", "LOCK AQUIRED");
			Send();
			unlock();
			//ofLogNotice("Loop", "UNLOCKED");
			ofSleepMillis(7);
		}
		else
		{
			if (device->Connect() < 0)
			{
				ofLogError("EtherDream", "Couldn't reconnect to device");
			}
			else
				isFound = true;
		}
	}
}

void EtherDreamController::Start()
{
	if (isThreadRunning())
		return;
	startThread();
}

void EtherDreamController::Stop()
{
	stopThread();
	device->Stop();
	device->Disconnect();
}

void EtherDreamController::Send()
{
	//ofLogNotice("Send", "START");
	if (!isFound || points.empty())
	{
		//ofLogNotice("Send", "NO POINTS");
		return;
	}
	//ofLogNotice("Send", "CAN SEND");
	if (waitBeforeSend)
	{
		//ofLogNotice("Send", "WAITING FOR READY");
		device->WaitForReady();
		//ofLogNotice("Send", "READY");
	}
	else if (!device->IsReady())
		return;
	//ofLogNotice("Send", "SENDING");

	//TODO: This cast might cause problems test for this in CTR
	int res = device->Write((EtherDreamPoint*)points.data(), points.size(), pps, 1);
	if (res != 0)
		ofLogError("EtherDream", "Write result %d", res);
	points.clear();
}

void EtherDreamController::AddPoints(const vector<ofxIlda::Point>& _points)
{
	if (_points.empty())
		return;
	ofLogNotice("AddPoints", "Aquiring lock");
	while (!lock())
		ofSleepMillis(21);
	ofLogNotice("AddPoints", "Lock Aquired");
	points.insert(points.end(), _points.begin(), _points.end());
	unlock();
	ofLogNotice("AddPoints", "Unlocked");
}

void EtherDreamController::AddPoints(const ofxIlda::Frame & frame)
{
	AddPoints(frame.getPoints());
}

void EtherDreamController::SetWaitBeforeSend(bool wait)
{
	while (!lock())
		ofSleepMillis(26);
	waitBeforeSend = wait;
	unlock();
}

bool EtherDreamController::GetWaitBeforeSend()
{
	return waitBeforeSend;
}

void EtherDreamController::SetAutoConnect(bool connect)
{
	while (!lock())
		ofSleepMillis(26);
	autoConnect = connect;
	unlock();
}

bool EtherDreamController::GetAutoConnect()
{
	return autoConnect;
}

void EtherDreamController::SetPPS(int _pps)
{
	while (!lock())
		ofSleepMillis(26);
	pps = _pps;
	unlock();
}

int EtherDreamController::GetPPS()
{
	return pps;
}

bool EtherDreamController::IsFound()
{
	return isFound;
}
