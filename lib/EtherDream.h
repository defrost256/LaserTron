#pragma once

#include <string>
#include "EtherDreamProtocol.h"
#include "ofConstants.h"
#include "ofThread.h"
#include "ofUtils.h"
#ifndef TARGET_WIN32

	//unix includes - works for osx should be same for *nix
	#include <ctype.h>
	#include <netdb.h>
	#include <string.h>
	#include <fcntl.h>
	#include <time.h>
	#include <errno.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/timeb.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <sys/ioctl.h>

	#ifndef TARGET_ANDROID
		#include <sys/signal.h>
	#else
		#include <signal.h>
	#endif
#else
	//windows includes
	#include <winsock2.h>
	#include <ws2tcpip.h>		// TCP/IP annex needed for multicasting
#endif

using namespace std;

struct EtherDreamPoint
{
	int16_t x;
	int16_t y;
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t i;
	uint16_t u1;
	uint16_t u2;
};

struct EtherDreamConnection
{
	int socket;
	char readBuffer[1024];
	int readBufferSize;
	dac_response response;
	long long lastAckTime;
	
	PACK(
		struct LocalBuffer
		{
			queue_command queue;
			data_command_header header;
			dac_point data[1000];
		};
	)
	LocalBuffer localBuffer;

	int isBeginSent;
	int ackBuffer[64];
	int ackBufferProduce;
	int ackBufferConsume;
	int unAckPoints;
	int pendingMetaAcks;
};

struct BufferItem
{
	dac_point data[16000];
	unsigned int points;
	unsigned int pps;
	unsigned int repeatCount;
	unsigned int idx;
};

enum DacState {
	ST_Disconnected,
	ST_Ready,
	ST_Running,
	ST_Broken,
	ST_Shutdown
};

class EtherDream : public ofThread
{
public:

	EtherDream(in_addr _addr, char* _macAddress, int _swRevision);
	~EtherDream();
	
	int Connect();
	void Disconnect();
	int Write(EtherDreamPoint* pts, int nPts, int pps, int reps);
	bool IsReady();
	void WaitForReady();
	void Stop();

	in_addr GetAddress();
	unsigned long GetAddressL();
	EtherDream* GetNextC();
	void SetNext(EtherDream* _next);
private:

	Poco::Event cond;
	BufferItem buffer[2];
	int frameBufferRead;
	int frameBufferFullness;
	int bounceCount;

	struct in_addr addr;
	EtherDreamConnection conn;
	unsigned long dacId;
	int swRevision;
	char macAddress[6];
	char version[32];

	DacState state;
	EtherDream* next;

	void threadedFunction() override;

	int Connect_Inner();
	int ReadBytes(char* buf, int len);
	int SendAll(char* data, int len);
	int ReadResponse();
	void DumpResponse();
	void Bail();
	int WaitForActivity(int uSecTimeout, bool writable);
	int CheckDataResponse();
	int GetAcks(int uSecTimeout);
	int SendData(dac_point* data, int nPoints, int rate);
	void Loop();

};

class DacListener : public ofThread
{
	EtherDream* dacList;
	int dacCount;
public:
	int GetDacCount();
	EtherDream* GetDac(int id);

private:
	void threadedFunction() override;

};
