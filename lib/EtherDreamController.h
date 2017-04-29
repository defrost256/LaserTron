#pragma once

#include "EtherDream.h"
#include "ofxIldaFrame.h"
#include "ofUtils.h"

using namespace std;

class EtherDreamController : public ofThread
{
public:
	EtherDreamController(EtherDream* device, bool startThread);
	~EtherDreamController();
	void threadedFunction() override;

	void Start();
	void Stop();

	void Send();

	void AddPoints(const vector<ofxIlda::Point> &_points);
	void AddPoints(const ofxIlda::Frame &frame);

	void SetWaitBeforeSend(bool wait);
	bool GetWaitBeforeSend();
	void SetAutoConnect(bool connect);
	bool GetAutoConnect();
	void SetPPS(int pps);
	int GetPPS();
	bool IsFound();

private:

	bool isFound;
	int pps;
	bool waitBeforeSend;
	bool autoConnect;

	EtherDream* device;
	vector<ofxIlda::Point> points;
};

