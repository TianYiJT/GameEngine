#pragma once
#include"Framework.h"
#include<thread>

using namespace std;

class AudioPlayer
{
public:
	static ALboolean ALStartUp() 
	{
		ALFWInit();
		ALFWprintf("PlayStatic Test Application\n");
		if (!MyInitOpenAL())
		{
			ALFWprintf("Failed to initialize OpenAL\n");
			ALFWShutdown();
			return 0;
		}
	}

	AudioPlayer()
	{
		alGenBuffers(1, &uiBuffer);
		alGenSources(1, &uiSource);
		mAudioThread = new thread(&AudioPlayer::Run,this);
	}

	AudioPlayer(const char* path) 
	{
		alGenBuffers(1, &uiBuffer);
		alGenSources(1, &uiSource);
		mAudioThread = new thread(&AudioPlayer::Run, this);
		if (!ALFWLoadWaveToBuffer(path, uiBuffer))
		{
			ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(path));
		}
		alGenSources(1, &uiSource);
		alSourcei(uiSource, AL_BUFFER, uiBuffer);
	}

	~AudioPlayer() 
	{
		alDeleteSources(1, &uiSource);
		alDeleteBuffers(1, &uiBuffer);
	}

	void Run() 
	{
		while (true) 
		{
			if (isPlay)
			{
				do
				{
					Sleep(100);
					alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
				} while (iState == AL_PLAYING);
				if (isLoop)
				{
					alSourcePlay(uiSource);
				}
			}
			Sleep(100);
		}
	}

	void Play() 
	{
		isPlay = true;
		alSourcePlay(uiSource);
	}

	void Stop() 
	{
		isPlay = false;
		alSourceStop(uiSource);
	}

	void SetLoop(bool Looping)
	{
		isLoop = Looping;
	}

private:
	static ALboolean MyInitOpenAL()
	{
		ALCdevice * p_device;
		ALCcontext* p_context;
		p_device = alcOpenDevice(NULL);
		if (p_device)
			p_context = alcCreateContext(p_device, NULL);
		if (p_context)
		{
			if (alcMakeContextCurrent(p_context))
				return true;
			else
				return false;
		}
		return false;
	}
	thread * mAudioThread;
//	string mPath;
	ALuint      uiBuffer;
	ALuint      uiSource;
	ALint       iState;
	bool isLoop = false;
	bool isPlay = false;
};