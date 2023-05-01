#pragma once

#include <stdbool.h>

typedef struct
{
	unsigned int handle;
} Texture;

bool TEXTURE_Load(const char* filename, Texture* texture);