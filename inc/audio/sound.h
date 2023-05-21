#pragma once

#include <xaudio2.h>

typedef struct
{
	IXAudio2SourceVoice* sourceVoice;
	void* data;
} Sound;