#include <Windows.h>
#include <d3dx9.h>

#include "component/renderer.h"

#include "render.h"
#include "mesh.h"
#include "object.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	(void)hPrevInstance;
	(void)lpCmdLine;

	if (!RENDER_Init(hInstance, nShowCmd))
	{
		MessageBoxA(NULL, "Failed to create renderer.", "Failed", MB_OK | MB_ICONERROR);
		return 1;
	}

	Object* object = OBJECT_NewObject("demo.mesh");

	object->transform->position->z = -200.f;
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

		OBJECT_Update(object);

		Sleep(10);
	}

	RENDER_Destroy();

	OBJECT_DestroyObject(object);

	return 0;
}