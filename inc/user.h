#pragma once

#include <stdbool.h>

#include "game.h"
#include "context/context.h"

enum UserTag {
	USERTAG_TRAP,
	USERTAG_OBJECTIVE,
	USERTAG_CHECKPOINT,
	USERTAG_CREDITS,
	USERTAG_JUMPER,
	USERTAG_EASTEREGG
};

void USER_Init(Game* g, Context* ctx);
void USER_Update(Game* g, Context* ctx);
void USER_Input(int key, bool isPressed);
const Vector* GetCameraPosition(void);