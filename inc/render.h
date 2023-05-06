#pragma once

#include <d3d9.h>
#include <stdbool.h>

bool RENDER_Init(HINSTANCE hInstance, int nShowCmd);
void RENDER_Destroy();
void RENDER_Render();

extern LPDIRECT3DDEVICE9 d3dDevice;