#include <stdlib.h>

#include "context/context.h"

Context* CONTEXT_New()
{
	Context* ctx = malloc(sizeof(Context));
	if (ctx == NULL) return NULL;

	ctx->time = TIME_New();
	return ctx;
}

void CONTEXT_Destroy(Context* ctx)
{
	TIME_Destroy(ctx->time);
	free(ctx);
}

void CONTEXT_Update(Context* ctx)
{
	TIME_Update(ctx->time);
}