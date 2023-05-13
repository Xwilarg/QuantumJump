#pragma once

#include <sys\timeb.h>

typedef struct {
	struct timeb lastUpdate;

	float deltaTime;
} Time;

Time* TIME_New(void);
void TIME_Destroy(Time* t);
void TIME_Update(Time* t);