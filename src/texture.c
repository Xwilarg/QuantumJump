#include <Windows.h>
#include <GL/GL.h>

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#include "texture.h"

#pragma pack(push, 1)
typedef struct
{
	uint16_t width;
	uint16_t height;
} TextureHeader;
#pragma pack(pop)

static void UploadTexture(Texture* texture, int width, int height, void* data);

bool TEXTURE_Load(const char* filename, Texture* texture)
{
	FILE* f;
	int err = fopen_s(&f, filename, "rb");
	if (err != 0)
	{
#pragma warning(suppress: 4996)
		assert(!strerror(err));
		return false;
	}

	// read the texture header
	TextureHeader header;
	fread(&header, sizeof(header), 1, f);

	unsigned int size = header.width * header.height * 3;
	void* data = malloc(size);

	if (!data)
	{
		return false;
	}

	fread(data, size, 1, f);
	UploadTexture(texture, header.width, header.height, data);

	// free the texture data
	free(data);

	return true;
}

static void UploadTexture(Texture* texture, int width, int height, void* data)
{
	GLuint textures;
	glGenTextures(1, &textures);

	glBindTexture(GL_TEXTURE_2D, textures);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	texture->handle = textures;
}