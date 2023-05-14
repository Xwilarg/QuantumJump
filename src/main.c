#include <Windows.h>
#include <d3dx9.h>
#include <stdbool.h>

#include "rendering/render.h"
#include "game.h"
#include "user.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	(void)hPrevInstance;
	(void)lpCmdLine;

	if (!RENDER_Init(hInstance, nShowCmd))
	{
		MessageBoxA(NULL, "Failed to create renderer.", "Failed", MB_OK | MB_ICONERROR);
		return 1;
	}

	Context* ctx = CONTEXT_New();
	Game* game = GAME_Init();

	USER_Init(game, ctx);
	//

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

		// begin the frame
		RENDER_Clear();

		CONTEXT_Update(ctx);
		USER_Update(game, ctx);
		GAME_Update(game, ctx);

		RENDER_Render();

		Sleep(10);
	}

	RENDER_Destroy();
	CONTEXT_Destroy(ctx);
	GAME_Destroy(game);

	return 0;
}