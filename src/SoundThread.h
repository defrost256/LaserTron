#pragma once

#include <mutex>
#include "ofSoundStream.h"

class InputHandler : public ofBaseSoundInput
{

public:
    
    float GetData()
    {
	//Data = Amplitude * sin(time);
	//time += speed;
	/*if (readBufferIdx < 0)
	    return 0;
	_mutex.lock();
	int ret = data[readBufferIdx];
	_mutex.unlock();
	return ret;*/
	return 0;
    }
private:

    void audioIn(ofSoundBuffer& buffer) override
    {
	data[writeBufferIdx] = buffer[0];
	SwapBuffer();
    }

    void SwapBuffer() {
	_mutex.lock();
	writeBufferIdx = (writeBufferIdx + 1) % 2;
	readBufferIdx = (readBufferIdx + 1) % 2;
	_mutex.unlock();
    }

    float data[2];
    int writeBufferIdx;
    int readBufferIdx;
    mutable std::mutex _mutex;

    float Data;
    float time = 0;
    int Amplitude = 50;
    float speed = 0.02;
};

class SoundThread
{
public:
    SoundThread(int deviceId = 0);
    ~SoundThread();

    float GetData();

private:
    
    ofSoundStream stream;
    InputHandler input;
};

