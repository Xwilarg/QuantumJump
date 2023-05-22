#pragma once

#include <d3dx9.h>

#include "utils/vector.h"

void MATRIX_Multiply(D3DMATRIX* out, D3DMATRIX* m1, D3DMATRIX* m2);
void MATRIX_Scale(D3DMATRIX* out, Vector* scale);
void MATRIX_Position(D3DMATRIX* out, Vector* position);
void MATRIX_Rotation(D3DMATRIX* out, Vector* rotation);

void MATRIX_Projection(D3DMATRIX* out, float nearZ, float farZ, float fov, float aspect);