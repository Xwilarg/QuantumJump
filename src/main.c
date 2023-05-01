#include <windows.h>

#include "render.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (!RENDER_Init())
	{
		MessageBoxA(NULL, "Failed to create renderer.", "Failed", MB_OK | MB_ICONERROR);
		return 1;
	}

	// enter the main loop
	while (true)
	{
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		RENDER_Render();

		Sleep(10);
	}

	return 0;
}