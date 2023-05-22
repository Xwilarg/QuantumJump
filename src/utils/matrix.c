#include "utils/matrix.h"

static void BuildIdentity(D3DMATRIX* out)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->m[i][j] = 0.f;

	// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixidentity#remarks
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

void MATRIX_Rotation(D3DMATRIX* out, Vector* rotation)
{
	BuildIdentity(out);

	float sroll = sinf(rotation->z);
	float croll = cosf(rotation->z);
	float spitch = sinf(rotation->x);
	float cpitch = cosf(rotation->x);
	float syaw = sinf(rotation->y);
	float cyaw = cosf(rotation->y);

	out->_11 = sroll * spitch * syaw + croll * cyaw;
	out->_12 = sroll * cpitch;
	out->_13 = sroll * spitch * cyaw - croll * syaw;

	out->_21 = croll * spitch * syaw - sroll * cyaw;
	out->_22 = croll * cpitch;
	out->_23 = croll * spitch * cyaw + sroll * syaw;

	out->_31 = cpitch * syaw;
	out->_32 = -spitch;
	out->_33 = cpitch * cyaw;
}

// https://learn.microsoft.com/en-us/windows/win32/direct3d9/projection-transform#setting-up-a-projection-matrix
void MATRIX_Projection(D3DMATRIX* out, float nearZ, float farZ, float fov, float aspect)
{
	BuildIdentity(out);

	float w = 1.f / (aspect * tanf(fov * 0.5f));
	float h = 1.f / tanf(fov * 0.5f);
	float q = farZ / (farZ - nearZ);

	out->_11 = w;
	out->_22 = h;
	out->_33 = q;
	out->_43 = -q * nearZ;
	out->_34 = 1.f;
}

// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh#remarks
void MATRIX_LookAt(D3DMATRIX* out, Vector* eye, Vector* at, Vector* up)
{
	BuildIdentity(out);

	Vector z = VECTOR_Magnitude(VECTOR_Substract(*at, *eye));
	Vector x = VECTOR_Magnitude(VECTOR_Cross(*up, z));
	Vector y = VECTOR_Cross(z, x);

	out->_11 = x.x;
	out->_12 = y.x;
	out->_13 = z.x;
	out->_21 = x.y;
	out->_22 = y.y;
	out->_23 = z.y;
	out->_31 = x.z;
	out->_32 = y.z;
	out->_33 = z.z;
	out->_41 = -VECTOR_Dot(x, *eye);
	out->_42 = -VECTOR_Dot(y, *eye);
	out->_43 = -VECTOR_Dot(z, *eye);
}