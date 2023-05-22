#include "utils/matrix.h"

static void BuildIdentity(D3DMATRIX* out)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->m[i][j] = 0.f;

	out->_11 = 1.f;
	out->_22 = 1.f;
	out->_33 = 1.f;
	out->_44 = 1.f;
}

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

void MATRIX_Scale(D3DMATRIX* out, Vector* scale)
{
	BuildIdentity(out);

	out->_11 = scale->x;
	out->_22 = scale->y;
	out->_33 = scale->z;
}

void MATRIX_Position(D3DMATRIX* out, Vector* position)
{
	BuildIdentity(out);

	out->_41 = position->x;
	out->_42 = position->y;
	out->_43 = position->z;
}