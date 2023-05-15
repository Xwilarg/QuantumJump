#pragma once

#include <stdbool.h>

#include "game.h"
#include "context/context.h"

enum UserTag {
	USERTAG_TRAP
};

void USER_Init(Game* g, Context* ctx);
void USER_Update(Game* g, Context* ctx);
void USER_Input(int key, bool isPressed);
const Vector* GetCameraPosition(void);