#pragma once

#include <d3d9.h>
#include <stdbool.h>

#include "object.h"

typedef struct
{
	float x, y, z;
	float u, v;
} RenderVertex;

#define RENDERFVF D3DFVF_XYZ | D3DFVF_TEX2

bool RENDER_Init(HINSTANCE hInstance, int nShowCmd);
void RENDER_Destroy();
void RENDER_Clear();
void RENDER_Render();

void RENDER_RenderObject(Mesh* mesh, Transform* t);

extern LPDIRECT3DDEVICE9 d3dDevice;