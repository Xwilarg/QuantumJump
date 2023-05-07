#include <Windows.h>
#include <d3d9.h>

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#include "rendering/texture.h"
#include "rendering/render.h"

#pragma pack(push, 1)
typedef struct
{
	uint16_t width;
	uint16_t height;
} TextureHeader;
#pragma pack(pop)

static bool UploadTexture(Texture* texture, int width, int height, void* data);

bool TEXTURE_Load(const char* filename, Texture* texture)
{
	FILE* f;
	if (fopen_s(&f, filename, "rb") != 0)
	{
		assert(!"Failed to open texture");
		return false;
	}

	// read the texture header
	TextureHeader header;
	fread(&header, sizeof(header), 1, f);

	unsigned int size = header.width * header.height * 4;
	void* data = malloc(size);

	if (!data)
	{
		return false;
	}

	fread(data, size, 1, f);

	if (!UploadTexture(texture, header.width, header.height, data))
	{
		free(data);
		return false;
	}

	// free the texture data
	free(data);

	fclose(f);

	return true;
}

void TEXTURE_Free(Texture* texture)
{
	texture->handle->lpVtbl->Release(texture->handle);
	texture->handle = NULL;
}

static bool UploadTexture(Texture* texture, int width, int height, void* data)
{
	LPDIRECT3DTEXTURE9 resource;

	if (FAILED(d3dDevice->lpVtbl->CreateTexture(d3dDevice, width, height, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &resource, NULL)))
	{
		return false;
	}

	D3DLOCKED_RECT rect;
	resource->lpVtbl->LockRect(resource, 0, &rect, NULL, NULL);

	// upload the texture data
	memcpy(rect.pBits, data, width * height * 4);

	resource->lpVtbl->UnlockRect(resource, 0);

	texture->handle = resource;
	return true;
}