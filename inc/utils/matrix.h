#pragma once

#include <d3dx9.h>

#include "utils/vector.h"

void MATRIX_Multiply(D3DMATRIX* out, D3DMATRIX* m1, D3DMATRIX* m2);
void MATRIX_Scale(D3DMATRIX* out, Vector* scale);
void MATRIX_Position(D3DMATRIX* out, Vector* position);