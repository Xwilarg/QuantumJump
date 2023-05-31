#include <assert.h>

#include "rendering/font.h"
#include "rendering/texture.h"
#include "rendering/render.h"

static Texture* font;

static RenderBuffer* verticeBuffer;

static int cursorX;
static int cursorY;
static int fontSizeX = FONT_CHAR_WIDTH;
static int fontSizeY = FONT_CHAR_HEIGHT;

static bool CreateBuffer()
{
	verticeBuffer = malloc(sizeof(RenderBuffer));

	if (!verticeBuffer)
	{
		return false;
	}

	// create a buffer large enough to hold some text quads
	if (FAILED(d3dDevice->lpVtbl->CreateVertexBuffer(d3dDevice, 2048, 0, RENDER2DFVF, D3DPOOL_DEFAULT, &verticeBuffer->resource, NULL)))
	{
		return false;
	}

	return true;
}

bool FONT_Init(void)
{
	font = malloc(sizeof(Texture));

	if (!font)
	{
		return false;
	}

	if (!TEXTURE_Load("res/fonts/default.tex", font))
	{
		assert(!"Failed to load font texture");

		return false;
	}

	if (!CreateBuffer())
	{
		assert(false);
		return false;
	}

	return true;
}

void FONT_SetCursor(int x, int y)
{
	cursorX = x;
	cursorY = y;
}

void FONT_SetSize(int sizeX, int sizeY)
{
	fontSizeX = sizeX;
	fontSizeY = sizeY;
}

static void CopyVertex(RenderBuffer* self, void* data, size_t size)
{
	LPDIRECT3DVERTEXBUFFER9 resource = (LPDIRECT3DVERTEXBUFFER9)self->resource;

	void* buffer;
	resource->lpVtbl->Lock(resource, 0, 0, &buffer, 0);

	memcpy((unsigned char*)buffer + self->size, data, size);
	self->size += size;

	resource->lpVtbl->Unlock(resource);
}

void FONT_Print(const char* text)
{
	//assert(strlen(text) * sizeof(RenderVertex2d) * 4 < 2048);

	const char* c = text;
	while (*c)
	{
		int chr = *c - FONT_START_CHAR;

		// calculate the position of the character in the bitmap
		int x = (chr * FONT_CHAR_WIDTH) % FONT_WIDTH;
		int y = ((chr * FONT_CHAR_WIDTH) / FONT_WIDTH) * FONT_CHAR_HEIGHT;

		// calculate the texture coordinates
		float u1 = (float)x / FONT_WIDTH;
		float v1 = (float)y / FONT_HEIGHT;
		float u2 = ((float)x + FONT_CHAR_WIDTH) / FONT_WIDTH;
		float v2 = ((float)y + FONT_CHAR_HEIGHT) / FONT_HEIGHT;

		RenderVertex2d vertices[] = {
			{(float)cursorX,				(float)cursorY,				0.f,	1.f,	u1, v1},
			{(float)cursorX + fontSizeX,	(float)cursorY,				0.f,	1.f,	u2, v1},
			{(float)cursorX,				(float)cursorY + fontSizeY,	0.f,	1.f,	u1, v2},
			{(float)cursorX + fontSizeX,	(float)cursorY + fontSizeY,	0.f,	1.f,	u2, v2},
		};

		// TODO optimize draw calls by filling the whole buffer first
		// but that won't work with a triangle strip
		verticeBuffer->size = 0;
		CopyVertex(verticeBuffer, &vertices, sizeof(vertices));

		RENDER_RenderTexture2d(font, verticeBuffer, 2);

		// move the text cursor
		cursorX += fontSizeX;
		c++;
	}
}

void FONT_Destroy(void)
{
	TEXTURE_Free(font);
	free(font);

	verticeBuffer->resource->lpVtbl->Release(verticeBuffer->resource);
	free(verticeBuffer);
}