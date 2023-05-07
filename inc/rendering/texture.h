#pragma once

#include <d3d9.h>
#include <stdbool.h>

typedef struct
{
	LPDIRECT3DBASETEXTURE9 handle;
} Texture;

bool TEXTURE_Load(const char* filename, Texture* texture);
void TEXTURE_Free(Texture* texture);