#pragma once

#include <stdbool.h>

#include "sound.h"

bool AUDIO_Init();
void AUDIO_Destroy();

Sound* AUDIO_Load(const char* soundPath);
void AUDIO_Play(Sound* sound);