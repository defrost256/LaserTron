#include "SoundThread.h"



SoundThread::SoundThread(int deviceId)
{
    /*stream.setDeviceID(deviceId);
    stream.setInput(input);
    stream.setup(0, 2, 44100, 256, 4);
    stream.start();*/
}


SoundThread::~SoundThread()
{
    /*stream.stop();*/
}

float SoundThread::GetData()
{
    return input.GetData();
}
