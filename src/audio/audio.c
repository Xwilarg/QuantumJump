#include <xaudio2.h>

#include <stdio.h>
#include <stdint.h>

#include "audio/audio.h"
#include "audio/sound.h"

static IXAudio2* xAudio2;
static IXAudio2MasteringVoice* masterVoice;

#pragma pack(push, 1)
typedef struct
{
	uint32_t size;
	uint16_t sampleRate;
	uint16_t blockAlign;
} SoundHeader;
#pragma pack(pop)

// Microsoft ADPCM
static ADPCMCOEFSET aCoef[] = {{256, 0}, {512, -256}, {0,0}, {192,64}, {240,0}, {460, -208}, {392,-232}};

bool AUDIO_Init()
{
	if (FAILED(XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return false;
	}

	if (FAILED(xAudio2->lpVtbl->CreateMasteringVoice(
		xAudio2, &masterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0)))
	{
		return false;
	}

#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debugConfiguration;
	memset(&debugConfiguration, 0, sizeof(debugConfiguration));

	debugConfiguration.TraceMask = XAUDIO2_LOG_INFO | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_FUNC_CALLS;
	debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;

	xAudio2->lpVtbl->SetDebugConfiguration(xAudio2, &debugConfiguration, NULL);
#endif

	return true;
}

void AUDIO_Destroy()
{
	masterVoice->lpVtbl->DestroyVoice(masterVoice);
	xAudio2->lpVtbl->Release(xAudio2);
}

Sound* AUDIO_Load(const char* soundPath, bool loop)
{
	Sound* sound = malloc(sizeof(Sound));
	if (sound == NULL) return false;

	FILE* f;
	if (fopen_s(&f, soundPath, "rb") != 0)
	{
		return NULL;
	}

	// read the sound header
	SoundHeader header;
	fread(&header, 1, sizeof(header), f);

	// allocate audio buffer
	void* data = malloc(header.size);

	if (!data)
	{
		free(sound);
		return NULL;
	}

	fread(data, 1, header.size, f);

	// create the xaudio buffer
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(buffer));

	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = header.size;
	buffer.pAudioData = data;
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	// create audio source
	WAVEFORMATEX* waveFormat = malloc(sizeof(ADPCMWAVEFORMAT) + sizeof(aCoef));

	if (!waveFormat)
	{
		return NULL;
	}

	waveFormat->wFormatTag = WAVE_FORMAT_ADPCM;
	waveFormat->nChannels = 1;

	// from the sound header
	waveFormat->nSamplesPerSec = header.sampleRate;
	waveFormat->nAvgBytesPerSec = header.sampleRate * header.blockAlign;
	waveFormat->nBlockAlign = header.blockAlign;

	waveFormat->wBitsPerSample = 4;
	waveFormat->cbSize = 32;

	// adpcm specific
	ADPCMWAVEFORMAT* adpcmWaveFormat = (ADPCMWAVEFORMAT*)waveFormat;
	adpcmWaveFormat->wSamplesPerBlock = waveFormat->nBlockAlign * 2 / waveFormat->nChannels - 12;
	adpcmWaveFormat->wNumCoef = 7;
	memcpy(adpcmWaveFormat->aCoef, aCoef, sizeof(aCoef));

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
	sound->data = data;

	free(waveFormat);
	fclose(f);

	return sound;
}

void AUDIO_Play(Sound* sound)
{
	sound->sourceVoice->lpVtbl->Start(sound->sourceVoice, 0, 0);
}

void AUDIO_Free(Sound* sound)
{
	sound->sourceVoice->lpVtbl->Stop(sound->sourceVoice, 0, 0);
	sound->sourceVoice->lpVtbl->DestroyVoice(sound->sourceVoice);
	
	free(sound->data);
	free(sound);
}