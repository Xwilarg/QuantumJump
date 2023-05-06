#include <Windows.h>

#include "render.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HWND hWnd;
static LPDIRECT3D9 d3d;

static int width;
static int height;

LPDIRECT3DDEVICE9 d3dDevice;

static void UpdateClientArea()
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	width = rect.right;
	height = rect.bottom;
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
		"Game",

		WS_OVERLAPPEDWINDOW,
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

	UpdateClientArea();

	return true;
}

static bool CreateDevice()
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

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		UpdateClientArea();
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

	ShowWindow(hWnd, nShowCmd);

	return true;
}

void RENDER_Render()
{
	d3dDevice->lpVtbl->Present(d3dDevice, NULL, NULL, NULL, NULL);
}

void RENDER_Destroy()
{
	d3dDevice->lpVtbl->Release(d3dDevice);
	d3d->lpVtbl->Release(d3d);

	DestroyWindow(hWnd);
}