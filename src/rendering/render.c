#include <Windows.h>
#include <d3d9.h>

#include "rendering/render.h"
#include "utils/matrix.h"
#include "utils/math.h"
#include "user.h"
#include "config.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HWND hWnd;
static LPDIRECT3D9 d3d;

static int width;
static int height;

LPDIRECT3DDEVICE9 d3dDevice;

static void UpdateClientArea(void)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	width = rect.right;
	height = rect.bottom;

	// update the projection matrix
	D3DMATRIX matProjection;
	MATRIX_Projection(&matProjection, 1.0f, 1000.f, (float)ToRadian(45), (float)width / (float)height);

	d3dDevice->lpVtbl->SetTransform(d3dDevice, D3DTS_PROJECTION, &matProjection);
}

static void BuildViewTransform(const Vector* position)
{
	Vector lookAt = VECTOR_New(position->x, position->y - 10.f, position->z - 10.f);
	Vector up = VECTOR_New(0.f, 1.f, 0.f);

	D3DMATRIX matView;
	MATRIX_LookAt(&matView, (Vector*)position, &lookAt, &up);

	d3dDevice->lpVtbl->SetTransform(d3dDevice, D3DTS_VIEW, &matView);
}

static bool InitWindow(HINSTANCE hInstance)
{
	// register the window class
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "static";

	RegisterClassA(&wc);

	// create the window
	hWnd = CreateWindowA(
		"static",
		CONFIG_GAME_NAME,

		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == NULL)
	{
		return false;
	}

	return true;
}

static bool CreateDevice(void)
{
	// create the dx9 object
	if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create the device
	if (FAILED(d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice)))
	{
		return false;
	}

	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_LIGHTING, false);
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_ZENABLE, true);

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		USER_Input(wParam, true);
		break;

	case WM_KEYUP:
		USER_Input(wParam, false);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

bool RENDER_Init(HINSTANCE hInstance, int nShowCmd)
{
	if (!InitWindow(hInstance))
	{
		return false;
	}

	if (!CreateDevice())
	{
		return false;
	}

	UpdateClientArea();

	ShowWindow(hWnd, nShowCmd);

	return true;
}

// Begin the frame
void RENDER_Clear(void)
{
	// clear the screen and the depth buffer
	d3dDevice->lpVtbl->Clear(d3dDevice, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);

	// begin a new scene
	d3dDevice->lpVtbl->BeginScene(d3dDevice);

	BuildViewTransform(GetCameraPosition());
}

// End the frame
void RENDER_Render(void)
{
	d3dDevice->lpVtbl->EndScene(d3dDevice);

	d3dDevice->lpVtbl->Present(d3dDevice, NULL, NULL, NULL, NULL);
}

void RENDER_RenderCube(Vector min, Vector max, Vector rotation)
{
	// Indra my love, please implement this
	(void)min;
	(void)max;
	(void)rotation;

	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

// Render an object
void RENDER_RenderMesh(Mesh* mesh, Transform *t)
{
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_FILLMODE, D3DFILL_SOLID);

	// position
	D3DMATRIX matPosition;
	MATRIX_Position(&matPosition, &t->position);

	// rotation
	D3DMATRIX matRotation;
	MATRIX_Rotation(&matRotation, &t->rotation);

	// scale
	D3DMATRIX matScale;
	MATRIX_Scale(&matScale, &t->scale);

	// rotation * scale * position
	D3DMATRIX transformRotScale;
	D3DMATRIX transformRotScalePos;
	MATRIX_Multiply(&transformRotScale, &matRotation, &matScale);
	MATRIX_Multiply(&transformRotScalePos, &transformRotScale, &matPosition);

	d3dDevice->lpVtbl->SetTransform(d3dDevice, D3DTS_WORLD, &transformRotScalePos);

	// set the vertex source and indices buffer
	d3dDevice->lpVtbl->SetFVF(d3dDevice, RENDERFVF);
	d3dDevice->lpVtbl->SetStreamSource(d3dDevice, 0, mesh->vertexBuffer, 0, sizeof(RenderVertex));
	d3dDevice->lpVtbl->SetIndices(d3dDevice, mesh->indexBuffer);

	d3dDevice->lpVtbl->SetTexture(d3dDevice, 0, mesh->texture->handle);

	d3dDevice->lpVtbl->DrawIndexedPrimitive(d3dDevice, D3DPT_TRIANGLELIST, 0, 0, mesh->numVertices, 0, mesh->numIndices);
}

void RENDER_RenderTexture2d(Texture* texture, RenderBuffer* vertices, int numPrimitives)
{
	// set the default render state
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_FILLMODE, D3DFILL_SOLID);

	// enable alpha blending
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_ALPHABLENDENABLE, true);

	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// set the vertex source
	d3dDevice->lpVtbl->SetFVF(d3dDevice, RENDER2DFVF);
	d3dDevice->lpVtbl->SetStreamSource(d3dDevice, 0, (LPDIRECT3DVERTEXBUFFER9)vertices->resource, 0, sizeof(RenderVertex2d));

	d3dDevice->lpVtbl->SetTexture(d3dDevice, 0, texture->handle);

	// draw our vertices as triangle strip
	d3dDevice->lpVtbl->DrawPrimitive(d3dDevice, D3DPT_TRIANGLESTRIP, 0, numPrimitives);

	// reset render state
	d3dDevice->lpVtbl->SetRenderState(d3dDevice, D3DRS_ALPHABLENDENABLE, false);
}

void RENDER_Destroy(void)
{
	d3dDevice->lpVtbl->Release(d3dDevice);
	d3d->lpVtbl->Release(d3d);

	DestroyWindow(hWnd);
}