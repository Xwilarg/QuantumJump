#pragma once

#include <Windows.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092

typedef size_t GLsizeiptrARB;
typedef ptrdiff_t GLintptrARB;

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

typedef void (APIENTRYP PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const void* data, GLenum usage);

#define getProcAddress(proc, type) proc = (type)/* no decltype in C */wglGetProcAddress(#proc);

extern PFNGLGENBUFFERSARBPROC	glGenBuffers;
extern PFNGLBINDBUFFERARBPROC	glBindBuffer;
extern PFNGLBUFFERDATAARBPROC	glBufferData;