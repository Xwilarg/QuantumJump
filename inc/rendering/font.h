#pragma once

#include <stdbool.h>

#define FONT_START_CHAR ' '
#define FONT_CHAR_WIDTH 16
#define FONT_CHAR_HEIGHT 32

#define FONT_WIDTH 256
#define FONT_HEIGHT 256

bool FONT_Init(void);
void FONT_Destroy(void);

void FONT_SetCursor(int x, int y);
void FONT_SetSize(int sizeX, int sizeY);
void FONT_Print(const char* text);