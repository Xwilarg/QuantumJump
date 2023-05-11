#pragma once

#include "object.h"
#include "context/context.h"

typedef struct {
	Object** objects;
} Game;

Game* GAME_Init();
void GAME_Destroy(Game* g);
void GAME_Update(Game* g, Context* ctx);
void GAME_AddObject(Game* g, Object* obj);