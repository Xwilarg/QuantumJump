#pragma once

#include <stdbool.h>

#include "sound.h"

bool AUDIO_Init(void);
void AUDIO_Destroy(void);

Sound* AUDIO_Load(const char* soundPath, bool loop);
void AUDIO_Play(Sound* sound);
void AUDIO_Free(Sound* sound);