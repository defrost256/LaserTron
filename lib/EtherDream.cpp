#include "EtherDream.h"

EtherDream::EtherDream(in_addr _addr, char * _macAddress, int _swRevision)
{
	addr = _addr;
	memcpy(macAddress, _macAddress, 6);
	dacId = (macAddress[3] << 16)
		| (macAddress[4] << 8)
		| macAddress[5];
	swRevision = _swRevision;
	state = ST_Disconnected;
}

EtherDream::~EtherDream()
{
}

void EtherDream::LogEtherDream(const string & module, const char* format, ...)
{
	va_list args;
	string time = ofGetTimestampString();
	const string timedModule = time.append(module);
	va_start(args, format);
	ofLogNotice(timedModule, format, args);
	va_end(args);
}

int EtherDream::Connect()
{
	frameBufferRead = 0;
	frameBufferFullness = 0;
	//TODO: Why not working idx is huge negative value 
	memset(buffer, sizeof(buffer), 0);
	buffer[0].idx = 0;
	buffer[1].idx = 0;

	if (Connect_Inner() < 0)
	{
		ofLogError("EtherDream", "Connection failed");
		return -1;
	}

	state = ST_Ready;

	startThread();

	LogEtherDream("EtherDream", "Connected");
	return 0;
}

void EtherDream::Disconnect()
{
	LogEtherDream("EtherDream", "Disconnecting");
	lock();
	if (state == ST_Ready)
		cond.set();
	state = ST_Shutdown;
	unlock();

	stopThread();
	closesocket(conn.socket);
}

int EtherDream::Write(EtherDreamPoint * pts, int nPts, int pps, int reps)
{
	if (nPts > 16000)
		nPts = 16000;

	if (!reps)
		return 0;
	lock();

	if (frameBufferFullness == 2)
	{
		unlock();
		ofLogError("EtherDream", "Not ready %d points, %d reps", nPts, reps);
		return -1;
	}
	BufferItem* next = &buffer[(frameBufferRead + frameBufferFullness) % 2];
	unlock();

	for (int i = 0; i < nPts; i++)
	{
		next->data[i].x = pts[i].x;
		next->data[i].y = pts[i].y;
		next->data[i].r = pts[i].r;
		next->data[i].g = pts[i].g;
		next->data[i].b = pts[i].b;
		next->data[i].i = pts[i].i;
		next->data[i].u1 = pts[i].u1;
		next->data[i].u2 = pts[i].u2;
		next->data[i].control = 0;
	}

	next->pps = pps;
	next->repeatCount = reps;
	next->points = nPts;

	lock();
	frameBufferFullness++;
	if (state == ST_Ready)
		cond.set();
	state = ST_Running;
	unlock();

	return 0;
}

bool EtherDream::IsReady()
{
	lock();
	bool ready = frameBufferFullness != 2;
	unlock();
	return ready;
}

void EtherDream::WaitForReady()
{
	//LogEtherDream("WaitForReady", "AQUIRING LOCK");
	lock();
	//LogEtherDream("WaitForReady", "LOCK AQUIRED");
	while (frameBufferFullness == 2)
	{
		//LogEtherDream("WaitForReady", "FULL BUFFER");
		unlock();
		//LogEtherDream("WaitForReady", "UNLOCK");
		//LogEtherDream("WaitForReady", "WAITING FOR CONDITION");
		cond.wait();
		//LogEtherDream("WaitForReady", "CONDITION MET");
		//LogEtherDream("WaitForReady", "AQUIRING LOCK");
		lock();
		//LogEtherDream("WaitForReady", "LOCK AQUIRED");
	}
	unlock();
	//LogEtherDream("WaitForReady", "UNLOCK");
}

void EtherDream::Stop()
{
	lock();
	if (state == ST_Running)
		buffer[frameBufferRead].repeatCount = 0;
	unlock();
}

in_addr EtherDream::GetAddress()
{
	return addr;
}

unsigned long EtherDream::GetAddressL()
{
	return addr.s_addr;
}

EtherDream * EtherDream::GetNextC()
{
	return next;
}

void EtherDream::SetNext(EtherDream * _next)
{
	next = _next;
}

void EtherDream::threadedFunction()
{
	Loop();
}

int EtherDream::Connect_Inner()
{
	memset(&conn, 0, sizeof(conn));

	conn.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (conn.socket < 0)
	{
		ofLogError("EtherDream", "Socket creation failed");
		return -1;
	}

	unsigned long nonblocking = 1;
#ifndef TARGET_WIN32
	ioctl(conn.socket, FIONBIO, &nonblocking);
#else
	ioctlsocket(conn.socket, FIONBIO, &nonblocking);
#endif // !TARGET_WIN32
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = this->addr.s_addr;
	addr.sin_port = htons(7765);

	connect(conn.socket, (sockaddr*)&addr, (int)sizeof(addr));
	if (errno && errno != EINPROGRESS)
	{
		ofLogError("EtherDream", "Connect %d", errno);
		Bail();
		return -1;
	}

	int res = WaitForActivity(2000000, true);
	if (res < 0)
	{
		ofLogError("EtherDream", "WFA failed");
		Bail();
		return -1;
	}
	if (res == 0)
	{
		ofLogError("EtherDream", "Conection to %s timed out", inet_ntoa(this->addr));
		Bail();
		return -1;
	}

	int error;
#ifndef TARGET_WIN32
	unsigned int len = sizeof(error);
#else
	int len = sizeof(error);
#endif // !TARGET_WIN32

	if (getsockopt(conn.socket, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0)
	{
		ofLogError("EtherDream", "Can't get error");
		Bail();
		return -1;
	}
	if (error)
	{
		ofLogError("EtherDream", "Connection error %d", error);
		Bail();
		return -1;
	}

	int ndelay = 1;
	if (setsockopt(conn.socket, IPPROTO_TCP, TCP_NODELAY, (char*)&ndelay, sizeof(ndelay)) < 0)
	{
		ofLogError("EtherDream", "Failed to set nodelay");
		Bail();
		return -1;
	}
	//LogEtherDream("READRESP", "CONNECTINNER_1");
	if (ReadResponse() < 0)
	{
		ofLogError("EtherDream", "Failed to read response");
		Bail();
		return -1;
	}

	char c = 'p';
	SendAll(&c, 1);
	//LogEtherDream("READRESP", "CONNECTINNER_2");
	if (ReadResponse() < 0)
	{
		ofLogError("EtherDream", "Failed to prepare ED");
		Bail();
		return -1;
	}
	DumpResponse();

	if (swRevision >= 2)
	{
		c = 'v';
		if (SendAll(&c, 1) < 0)
		{
			ofLogError("EtherDream", "Can't get version");
			Bail();
			return -1;
		}
		res = ReadBytes(version, sizeof(version));
		if (res < 0)
			return res;
	}
	else
		strcpy(version, "[OLD]");

	LogEtherDream("EtherDream", "Dac Connected version:  %.*s", sizeof(version), version);
	return 0;
}

int EtherDream::ReadBytes(char * buf, int len)
{
	while (conn.readBufferSize < len)
	{
		int res = WaitForActivity(2000000, false);
		if (res < 0)
		{
			ofLogError("EtherDream", "WFA failed");
			return res;
		}
		if (res == 0)
		{
			ofLogError("EtherDream", "WFA Timeout");
			return -1;
		}

		res = recv(conn.socket, conn.readBuffer + conn.readBufferSize, len - conn.readBufferSize, 0);
		if (res <= 0)
		{
			ofLogError("EtherDream", "Failed to receive msg");
			return -1;
		}

		conn.readBufferSize += res;
	}

	memcpy(buf, conn.readBuffer, len);
	if (conn.readBufferSize > len)
		memmove(conn.readBuffer, conn.readBuffer + len, conn.readBufferSize - len);
	conn.readBufferSize -= len;
	return 0;
}

int EtherDream::SendAll(char * data, int len)
{
	do
	{
		int res = WaitForActivity(100000, true);
		if (res < 0)
		{
			ofLogError("EtherDream", "WFA failed");
			return -1;
		}
		if (res == 0)
		{
			ofLogError("EtherDream", "WFA Timeout");
			return -1;
		}

		res = send(conn.socket, data, len, 0);
		if (res < 0)
		{
			ofLogError("EtherDream", "Send failed");
			return -1;
		}

		len -= res;
		data += res;
	} while (len);

	return 0;
}

int EtherDream::ReadResponse()
{
	int res = ReadBytes((char*)&conn.response, sizeof(conn.response));
	if (res < 0)
		return res;
	LogEtherDream("EtherDreamDebug", "ReadResponse - PointCount : %d, BufferFullness : %d, PlaybackFlags : %x, Playback State: %d",conn.response.dac_status.point_count, conn.response.dac_status.buffer_fullness, conn.response.dac_status.playback_flags, conn.response.dac_status.playback_state);
	if (conn.response.response != 'a')
		ofLogError("EtherDream", "%c Not Acknowledged (%c)", conn.response.command, conn.response.response);
	conn.lastAckTime = ofGetElapsedTimeMicros();
	return 0;
}

void EtherDream::DumpResponse()
{
	ofLogError("EtherDream", "DumpResponse not implemented");
}

void EtherDream::Bail()
{
	closesocket(conn.socket);
}

int EtherDream::WaitForActivity(int uSecTimeout, bool writable)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(conn.socket, &set);
	timeval t;
	t.tv_sec = uSecTimeout / 1000000;
	t.tv_usec = uSecTimeout % 1000000;
	LogEtherDream("WaitForActivity", "Start");
	int res = select(conn.socket + 1, (writable ? nullptr : &set), (writable ? &set : nullptr), &set, (uSecTimeout == 0 ? nullptr : &t));
	LogEtherDream("WaitForActivity_end", "End");
	if (res < 0)
		ofLogError("EtherDream", "WFA - Select failed");
	return res;
}

int EtherDream::CheckDataResponse()
{
	//LogEtherDream("CheckDataResponse", "Start");
	if (conn.response.dac_status.playback_state == 0)
		conn.isBeginSent = 0;
	//LogEtherDream("CheckDataResponse_Start", "AckBuffer: (%d, %d)", conn.ackBufferProduce, conn.ackBufferConsume);
	if (conn.response.command == 'd')
	{
		//LogEtherDream("CheckDataResponse_D", "AckBuffer: (%d, %d)", conn.ackBufferProduce, conn.ackBufferConsume);
		if (conn.ackBufferProduce == conn.ackBufferConsume)
		{
			ofLogError("EtherDream", "Unexpected data ack");
			return -1;
		}
		conn.unAckPoints -= conn.ackBuffer[conn.ackBufferConsume];
		conn.ackBufferConsume = (conn.ackBufferConsume + 1) % 64;
		//LogEtherDream("CheckDataResponse_End", "AckBuffer: (%d, %d)", conn.ackBufferProduce, conn.ackBufferConsume);
	}
	else
	{
		//LogEtherDream("CheckDataResponse", "Consumed Ack for %c", conn.response.command);
		conn.pendingMetaAcks--;
	}

	if (conn.response.response != 'a' && conn.response.response != 'I')
	{
		ofLogError("EtherDream", "ACK for '%c' got '%c'", conn.response.command, conn.response.response);
		return -1;
	}
	return 0;
}

int EtherDream::GetAcks(int uSecTimeout)
{
	while (conn.pendingMetaAcks || (conn.ackBufferProduce != conn.ackBufferConsume))
	{
		int res = WaitForActivity(uSecTimeout, false);
		if (res <= 0)
			return res;
		//LogEtherDream("READRESP", "GETACKS");
		if ((res = ReadResponse()) < 0)
			return res;
		if ((res = CheckDataResponse()) < 0)
			return res;
	}
	return 0;
}

int EtherDream::SendData(dac_point * data, int nPoints, int rate)
{
	int res;
	const dac_status* st = &conn.response.dac_status;

	if (st->playback_state == 0) // Need prepare
	{
		char c = 'p';
		if ((res = SendAll(&c, 1)) < 0)
			return res;
		//LogEtherDream("READRESP", "SENDDATA_P");
		if ((res = ReadResponse()) < 0)
			return res;
		conn.pendingMetaAcks++;

		while (conn.pendingMetaAcks)
		{
			//LogEtherDream("SendData_GETMETAACK", "(%d)", conn.pendingMetaAcks);
			int tmpResp = GetAcks(1500);
			//LogEtherDream("SendData_GETMETAACK", "R:%d", tmpResp);
		}

	}

	if (st->buffer_fullness > 1200 && st->playback_state == 1 && !conn.isBeginSent) // Need begin
	{
		begin_command b;
		b.command = 'b';
		b.point_rate = rate;
		b.low_water_mark = 0;

		if ((res = SendAll((char*)&b, sizeof(b))) < 0)
			return res;
		LogEtherDream("READRESP", "SENDDATA_B");
		if ((res = ReadResponse()) < 0)
			return res;

		conn.isBeginSent = 1;
		conn.pendingMetaAcks++;
	}

	LogEtherDream("SendData_GETACK", "Ack");
	if ((res = GetAcks(0)) < 0)
		return res;
	if (nPoints <= 0)
		return 0;
	
	conn.localBuffer.queue.command = 'q';
	conn.localBuffer.queue.point_rate = rate;

	conn.localBuffer.header.command = 'd';
	conn.localBuffer.header.npoints = nPoints;

	memcpy(conn.localBuffer.data, data, nPoints * sizeof(dac_point));
	conn.localBuffer.data[0].control |= 0x8000;	//Rate changed flag

	if ((res = SendAll((char*)&conn.localBuffer, 8 + nPoints * sizeof(dac_point))) < 0)
		return res;

	conn.pendingMetaAcks++;
	conn.ackBuffer[conn.ackBufferProduce] = nPoints;
	conn.ackBufferProduce = (conn.ackBufferProduce + 1) % 64;
	conn.unAckPoints += nPoints;

	return 0;
}

void EtherDream::Loop()
{
	int res = 0;
	//LogEtherDream("EtherDream_Loop", "Aquiring lock (start)");
	lock();
	//LogEtherDream("EtherDream_Loop", "Lock aquired (start)");
	while (true)
	{
		int state;
		//LogEtherDream("EtherDream_Loop", "Waiting for Ready");
		while ((state = this->state) == ST_Ready)
		{
			unlock();
			cond.wait();
			lock();
		}
		//LogEtherDream("EtherDream_Loop", "Ready");
		unlock();
		//LogEtherDream("EtherDream_Loop", "Unlocked");

		if (state != ST_Running)
			break;

		BufferItem* b = &buffer[frameBufferRead];
		int cap;
		int expextedFullness, expectedUsed;
		//LogEtherDream("EtherDream_Loop", "Computing Buffer");
		while (true)
		{
			res = 0;

			long long timeDiff = ofGetElapsedTimeMicros() - conn.lastAckTime;
			expectedUsed = timeDiff * b->pps / 1000000;
			if (conn.response.dac_status.playback_state != 2)
				expectedUsed = 0;
			expextedFullness = conn.response.dac_status.buffer_fullness + conn.unAckPoints - expectedUsed;

			cap = 1700 - expextedFullness;

			if (cap > 40)
				break;
			if (conn.response.dac_status.playback_state != 2)
			{
				sleep(1);
				break;
			}

			int diff = 40 - cap;
			int waitTime = 500 + (1000000 * diff / b->pps);

			sleep(waitTime / 1000);
			if ((res = GetAcks(0)) < 0)
			{
				//LogEtherDream("Loop_Getack", "Result:%d", res);
				break;
			}
			//else
				//LogEtherDream("Loop_Getack", "Result:%d", res);

		}
		//LogEtherDream("EtherDream_Loop", "Buffer computed with %d", res);
		if (res < 0)
			break;

		int bLeft = b->points - b->idx;
		if (cap > bLeft)
			cap = bLeft;

		// TODO: Is this neccessary?
		if (cap > 80)
			cap = 80;
		//LogEtherDream("EtherDream_Loop", "Sending Data");
		res = SendData(b->data + b->idx, cap, b->pps);
		//LogEtherDream("EtherDream_Loop", "Data Sent (%d)", res);
		if (res < 0)
			break;

		//LogEtherDream("EtherDream_Loop", "Aquiring lock (Loop)");
		lock();
		//LogEtherDream("EtherDream_Loop", "Lock Aquired (Loop)");
		b->idx += cap;
		if (b->idx < b->points)
			continue;
		//LogEtherDream("EtherDream_Loop", "Swapping buffer");
		b->idx = 0;

		if (b->repeatCount > 1)
			b->repeatCount--;
		else if (frameBufferFullness > 1)
		{
			frameBufferRead++;
			if (frameBufferRead >= 2)
				frameBufferRead = 0;
			//LogEtherDream("EtherDream_Loop", "Setting wait cond");
			cond.set();
		}
		else if (b->repeatCount >= 0)
			state = ST_Ready;
	}

	state = ST_Shutdown;
	return;
}

void DacListener::threadedFunction()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		ofLogError("EtherDream", "Socket not created");
		return;
	}

	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
	{
		ofLogError("EtherDream", "Socket option not modified");
		return;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7654);
#ifndef TARGET_WIN32
	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
#else
	if(::bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
#endif // !TARGET_WIN32
	{
		ofLogError("EtherDream", "Bind failed");
		return;
	}
	EtherDream::LogEtherDream("EtherDream", "Listening for DACs");

	while (1)
	{
		sockaddr_in src;
		dac_broadcast buf;
#ifndef TARGET_WIN32
		unsigned int srcLen = sizeof(src);
#else
		int srcLen = sizeof(src);
#endif // !TARGET_WIN32

		int len = recvfrom(sock, (char*)&buf, sizeof(buf), 0, (sockaddr*)&src, &srcLen);
		if (len < 0)
		{
			ofLogError("EtherDream", "Failed recvfrom");
			return;
		}

		lock();
		EtherDream* p = dacList;
		while (p)
		{
			if (p->GetAddressL() == src.sin_addr.s_addr)
				break;
			p = p->GetNextC();
		}

		if (p && (p->GetAddressL() == src.sin_addr.s_addr))
		{
			unlock();
			continue;
		}

		unlock();

		EtherDream* newDac = new EtherDream(src.sin_addr, (char*)buf.mac_address, buf.sw_revision);

		dacCount++;

		EtherDream::LogEtherDream("EtherDream", "Found new Dac @ %s", inet_ntoa(src.sin_addr));

		lock();
		newDac->SetNext(dacList);
		dacList = newDac;
		unlock();
	}

	EtherDream::LogEtherDream("EtherDream", "Exiting");
}

int DacListener::GetDacCount()
{
	return dacCount;
}

EtherDream * DacListener::GetDac(int id)
{
	if(id < dacCount)
		return dacList + id;
	ofLogError("EtherDream", "No such EtherDream exists");
	return nullptr;
}
