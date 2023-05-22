#include <stdlib.h>

#include "context/time.h"

Time* TIME_New(void)
{
	Time* t = malloc(sizeof(Time));
	if (t == NULL) return NULL;

	ftime(&t->lastUpdate);
	t->deltaTime = 0.f;
	t->timeSinceStart = 0.f;
	return t;
}

void TIME_Destroy(Time* t)
{
	free(t);
}

void TIME_Update(Time* t)
{
	struct timeb newTime;
	ftime(&newTime);
	t->deltaTime = (float)(1000.0 * (newTime.time - t->lastUpdate.time) + (newTime.millitm - t->lastUpdate.millitm)) / 1000.f;
	t->lastUpdate = newTime;
	t->timeSinceStart += t->deltaTime;
}