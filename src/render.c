#include <Windows.h>
#include <GL/GL.h>

#undef CreateWindow

#include "render.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HWND hWnd;
static HDC hDC;

static HGLRC context;

static bool CreateWindow()
{
	// register the window class
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = "static";

	RegisterClassA(&wc);

	// create the window
	hWnd = CreateWindowA(
		"static",
		"Game",

		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		960, 540,

		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	if (hWnd == NULL)
	{
		return false;
	}

	hDC = GetDC(hWnd);

	return true;
}

static bool CreateContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int format = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, format, &pfd);

	// create the context
	context = wglCreateContext(hDC);

	if (context == NULL)
	{
		return false;
	}

	wglMakeCurrent(hDC, context);

	return true;
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

bool RENDER_Init()
{
	if (!CreateWindow())
	{
		return false;
	}

	if (!CreateContext())
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);

	return true;
}

void RENDER_Render()
{
	SwapBuffers(hDC);
}