#include <Windows.h>
#include <d3dx9.h>

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

	// update the projection matrix
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45), (float)width / (float)height, 1.0f, 1000.f);

	d3dDevice->lpVtbl->SetTransform(d3dDevice, D3DTS_PROJECTION, &matProjection);
}

static void BuildViewTransform(Vector* position)
{
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 up;

	lookAt.x = 0.f; lookAt.y = 0.f; lookAt.z = 0.f;
	up.x = 0.f; up.y = 1.f; up.z = 0.f;

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, (D3DXVECTOR3*)position, &lookAt, &up);

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

	UpdateClientArea();

	Vector camera;
	camera.x = 0.f; camera.y = 0.f; camera.z = 10.f;
	BuildViewTransform(&camera);

	ShowWindow(hWnd, nShowCmd);

	return true;
}

// Begin the frame
void RENDER_Clear()
{
	// clear the screen and the depth buffer
	d3dDevice->lpVtbl->Clear(d3dDevice, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);

	// begin a new scene
	d3dDevice->lpVtbl->BeginScene(d3dDevice);
}

// End the frame
void RENDER_Render()
{
	d3dDevice->lpVtbl->EndScene(d3dDevice);

	d3dDevice->lpVtbl->Present(d3dDevice, NULL, NULL, NULL, NULL);
}

// Render an object
void RENDER_RenderObject(Object* object)
{
	Mesh* mesh = object->renderer->mesh;

	// transform the object
	//

	// position
	D3DMATRIX matPosition;
	D3DXMatrixTranslation(&matPosition, object->transform->position->x, object->transform->position->y, object->transform->position->z);

	// rotation
	D3DMATRIX matRotation;
	D3DXMatrixRotationYawPitchRoll(&matRotation, object->transform->rotation->y, object->transform->rotation->x, object->transform->rotation->z);

	D3DMATRIX matTransform;
	D3DXMatrixMultiply(&matTransform, &matRotation, &matPosition);

	//TODO: Handle scale

	d3dDevice->lpVtbl->SetTransform(d3dDevice, D3DTS_WORLD, &matTransform);

	// set the vertex source and indices buffer
	d3dDevice->lpVtbl->SetFVF(d3dDevice, RENDERFVF);
	d3dDevice->lpVtbl->SetStreamSource(d3dDevice, 0, mesh->vertexBuffer, 0, sizeof(RenderVertex));
	d3dDevice->lpVtbl->SetIndices(d3dDevice, mesh->indexBuffer);

	d3dDevice->lpVtbl->DrawIndexedPrimitive(d3dDevice, D3DPT_TRIANGLELIST, 0, 0, mesh->numVertices, 0, mesh->numIndices);
}

void RENDER_Destroy()
{
	d3dDevice->lpVtbl->Release(d3dDevice);
	d3d->lpVtbl->Release(d3d);

	DestroyWindow(hWnd);
}