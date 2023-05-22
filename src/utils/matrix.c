#include "utils/matrix.h"

void MATRIX_Multiply(D3DMATRIX* out, D3DMATRIX* m1, D3DMATRIX* m2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float sum = 0.f;

			for (int k = 0; k < 4; k++)
				sum += m1->m[i][k] * m2->m[k][j];

			out->m[i][j] = sum;
		}
	}
}