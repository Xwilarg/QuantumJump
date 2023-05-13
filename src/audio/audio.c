#include <InitGuid.h>
#include <xaudio2.h>

#include <stdio.h>

#include "audio/audio.h"
#include "audio/sound.h"

static IXAudio2* xAudio2;
static IXAudio2MasteringVoice* masterVoice;

bool AUDIO_Init()
{
#ifdef _DEBUG
	int flags = XAUDIO2_DEBUG_ENGINE;
#else
	int flag = 0;
#endif

	if (FAILED(XAudio2Create(&xAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return false;
	}

	if (FAILED(xAudio2->lpVtbl->CreateMasteringVoice(
		xAudio2, &masterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL)))
	{
		return false;
	}

#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debugConfiguration;
	memset(&debugConfiguration, 0, sizeof(debugConfiguration));

	debugConfiguration.TraceMask = XAUDIO2_LOG_INFO | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_ERRORS;

	xAudio2->lpVtbl->SetDebugConfiguration(xAudio2, &debugConfiguration, NULL);
#endif

	return true;
}

void AUDIO_Destroy()
{
	masterVoice->lpVtbl->DestroyVoice(masterVoice);
	xAudio2->lpVtbl->Release(xAudio2);
}

Sound* AUDIO_Load(const char* soundPath)
{
	Sound* sound = malloc(sizeof(Sound));
	if (sound == NULL) return false;

	FILE* f;
	if (fopen_s(&f, soundPath, "rb") != 0)
	{
		return NULL;
	}

	// read the size
	unsigned int size;
	fread(&size, 1, sizeof(unsigned int), f);

	// allocate audio buffer
	void* data = malloc(size);

	if (!data)
	{
		free(sound);
		return NULL;
	}

	fread(data, 1, size, f);

	// create the xaudio buffer
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(buffer));

	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = size;
	buffer.pAudioData = data;

	// create audio source
	WAVEFORMAT* waveFormat = malloc(sizeof(PCMWAVEFORMAT));

	if (!waveFormat)
	{
		return NULL;
	}

	// TODO
	waveFormat->wFormatTag = WAVE_FORMAT_PCM;
	waveFormat->nChannels = 1;
	waveFormat->nSamplesPerSec = 44100;
	waveFormat->nAvgBytesPerSec = 88200;
	waveFormat->nBlockAlign = 2;

	PCMWAVEFORMAT* pcmWaveFormat = (PCMWAVEFORMAT*)waveFormat;
	pcmWaveFormat->wBitsPerSample = 16;

	IXAudio2SourceVoice* sourceVoice;
	if (FAILED(xAudio2->lpVtbl->CreateSourceVoice(xAudio2, &sourceVoice, waveFormat, 0, 1.0, NULL, NULL, NULL)))
	{
		return NULL;
	}

	if (FAILED(sourceVoice->lpVtbl->SubmitSourceBuffer(sourceVoice, &buffer, NULL)))
	{
		return NULL;
	}

	sound->sourceVoice = sourceVoice;

	return sound;
}

void AUDIO_Play(Sound* sound)
{
	sound->sourceVoice->lpVtbl->Start(sound->sourceVoice, 0, 0);
}