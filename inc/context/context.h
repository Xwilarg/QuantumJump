#pragma once

#include "context/time.h"

typedef struct {
	Time* time;
} Context;

Context* CONTEXT_New();
void CONTEXT_Destroy(Context* ctx);
void CONTEXT_Update(Context* ctx);