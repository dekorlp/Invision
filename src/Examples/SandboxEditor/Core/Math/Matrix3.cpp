#include "Matrix3.h"

#include <cmath>
#include <cstring>
#include <tuple>

#include "Vector3.h"


Matrix3::Matrix3() :
	a0(1), a1(0), a2(0),
	a3(0), a4(1), a5(0),
	a6(0), a7(0), a8(1)
{

}

Matrix3::Matrix3(float val) :
	a0(val), a1(0), a2(0),
	a3(0), a4(val), a5(0),
	a6(0), a7(0), a8(val)
{

}

#ifdef ROWMAJOR
Matrix3::Matrix3(float e00, float e01, float e02,
	float e10, float e11, float e12,
	float e20, float e21, float e22)
	: a0(e00), a1(e01), a2(e02),
	a3(e10), a4(e11), a5(e12),
	a6(e20), a7(e21), a8(e22)
{

}
#else
Matrix3::Matrix3(float e00, float e01, float e02,
	float e10, float e11, float e12,
	float e20, float e21, float e22)
	: a0(e00), a1(e10), a2(e20),
	a3(e01), a4(e11), a5(e21),
	a6(e02), a7(e12), a8(e22)
{

}
#endif 

float& Matrix3::At(int i, int j)
{
	return aa[i][j];
}

bool Matrix3::operator==(Matrix3 const& rhs) const
{
	return rhs.a0 == this->a0 && rhs.a1 == this->a1 && rhs.a2 == this->a2 && rhs.a3 == this->a3
		&& rhs.a4 == this->a4 && rhs.a5 == this->a5 && rhs.a6 == this->a6 && rhs.a7 == this->a7
		&& rhs.a8 == this->a8;
}

Matrix3::Matrix3(float* e)
{
	memcpy(a, e, sizeof(float) * 9);
}

Matrix3 Matrix3::operator*(Matrix3 const& rhs) const
{

	float out[9];

	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[3], rhs.a[6], 0);
	__m128 A1 = _mm_set_ps(rhs.a[1], rhs.a[4], rhs.a[7], 0);
	__m128 A2 = _mm_set_ps(rhs.a[2], rhs.a[5], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_mul_ps(B0, A0); __m128 B0A1 = _mm_mul_ps(B0, A1); __m128 B0A2 = _mm_mul_ps(B0, A2);
	__m128 B1A0 = _mm_mul_ps(B1, A0); __m128 B1A1 = _mm_mul_ps(B1, A1); __m128 B1A2 = _mm_mul_ps(B1, A2);
	__m128 B2A0 = _mm_mul_ps(B2, A0); __m128 B2A1 = _mm_mul_ps(B2, A1); __m128 B2A2 = _mm_mul_ps(B2, A2);

	__m128 HAddB0A0 = _mm_hadd_ps(B0A0, B0A0); __m128 HAddB0A1 = _mm_hadd_ps(B0A1, B0A1); __m128 HAddB0A2 = _mm_hadd_ps(B0A2, B0A2);
	__m128 HAddB1A0 = _mm_hadd_ps(B1A0, B1A0); __m128 HAddB1A1 = _mm_hadd_ps(B1A1, B1A1); __m128 HAddB1A2 = _mm_hadd_ps(B1A2, B1A2);
	__m128 HAddB2A0 = _mm_hadd_ps(B2A0, B2A0); __m128 HAddB2A1 = _mm_hadd_ps(B2A1, B2A1); __m128 HAddB2A2 = _mm_hadd_ps(B2A2, B2A2);

	out[0] = _mm_hadd_ps(HAddB0A0, HAddB0A0).m128_f32[0]; out[1] = _mm_hadd_ps(HAddB0A1, HAddB0A1).m128_f32[0]; out[2] = _mm_hadd_ps(HAddB0A2, HAddB0A2).m128_f32[0];
	out[3] = _mm_hadd_ps(HAddB1A0, HAddB1A0).m128_f32[0]; out[4] = _mm_hadd_ps(HAddB1A1, HAddB1A1).m128_f32[0]; out[5] = _mm_hadd_ps(HAddB1A2, HAddB1A2).m128_f32[0];
	out[6] = _mm_hadd_ps(HAddB2A0, HAddB2A0).m128_f32[0]; out[7] = _mm_hadd_ps(HAddB2A1, HAddB2A1).m128_f32[0]; out[8] = _mm_hadd_ps(HAddB2A2, HAddB2A2).m128_f32[0];

	return Matrix3(out[0], out[3], out[6],
		out[1], out[4], out[7],
		out[2], out[5], out[8]);
}

Matrix3 Matrix3::operator+(Matrix3 const& rhs) const
{
	float out[9];
	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[1], rhs.a[2], 0);
	__m128 A1 = _mm_set_ps(rhs.a[3], rhs.a[4], rhs.a[5], 0);
	__m128 A2 = _mm_set_ps(rhs.a[6], rhs.a[7], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_add_ps(B0, A0);
	__m128 B1A1 = _mm_add_ps(B1, A1);
	__m128 B2A2 = _mm_add_ps(B2, A2);

	out[0] = B0A0.m128_f32[3]; out[1] = B0A0.m128_f32[2]; out[2] = B0A0.m128_f32[1];
	out[3] = B1A1.m128_f32[3]; out[4] = B1A1.m128_f32[2]; out[5] = B1A1.m128_f32[1];
	out[6] = B2A2.m128_f32[3]; out[7] = B2A2.m128_f32[2]; out[8] = B2A2.m128_f32[1];

	return Matrix3((float*)out);
}

Matrix3 Matrix3::operator-(Matrix3 const& rhs) const
{
	float out[9];
	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[1], rhs.a[2], 0);
	__m128 A1 = _mm_set_ps(rhs.a[3], rhs.a[4], rhs.a[5], 0);
	__m128 A2 = _mm_set_ps(rhs.a[6], rhs.a[7], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_sub_ps(B0, A0);
	__m128 B1A1 = _mm_sub_ps(B1, A1);
	__m128 B2A2 = _mm_sub_ps(B2, A2);

	out[0] = B0A0.m128_f32[3]; out[1] = B0A0.m128_f32[2]; out[2] = B0A0.m128_f32[1];
	out[3] = B1A1.m128_f32[3]; out[4] = B1A1.m128_f32[2]; out[5] = B1A1.m128_f32[1];
	out[6] = B2A2.m128_f32[3]; out[7] = B2A2.m128_f32[2]; out[8] = B2A2.m128_f32[1];

	return Matrix3((float*)out);
}

Matrix3 Matrix3::operator+=(Matrix3 const& rhs) const
{
	float out[9];
	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[1], rhs.a[2], 0);
	__m128 A1 = _mm_set_ps(rhs.a[3], rhs.a[4], rhs.a[5], 0);
	__m128 A2 = _mm_set_ps(rhs.a[6], rhs.a[7], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_add_ps(B0, A0);
	__m128 B1A1 = _mm_add_ps(B1, A1);
	__m128 B2A2 = _mm_add_ps(B2, A2);

	out[0] = B0A0.m128_f32[3]; out[1] = B0A0.m128_f32[2]; out[2] = B0A0.m128_f32[1];
	out[3] = B1A1.m128_f32[3]; out[4] = B1A1.m128_f32[2]; out[5] = B1A1.m128_f32[1];
	out[6] = B2A2.m128_f32[3]; out[7] = B2A2.m128_f32[2]; out[8] = B2A2.m128_f32[1];

	memcpy((void*)this->a, &Matrix3(
		out[0], out[3], out[6],
		out[1], out[4], out[7],
		out[2], out[5], out[8]), sizeof(float) * 9);

	return *this;
}

Matrix3 Matrix3::operator-=(Matrix3 const& rhs) const
{
	float out[9];
	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[1], rhs.a[2], 0);
	__m128 A1 = _mm_set_ps(rhs.a[3], rhs.a[4], rhs.a[5], 0);
	__m128 A2 = _mm_set_ps(rhs.a[6], rhs.a[7], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_sub_ps(B0, A0);
	__m128 B1A1 = _mm_sub_ps(B1, A1);
	__m128 B2A2 = _mm_sub_ps(B2, A2);

	out[0] = B0A0.m128_f32[3]; out[1] = B0A0.m128_f32[2]; out[2] = B0A0.m128_f32[1];
	out[3] = B1A1.m128_f32[3]; out[4] = B1A1.m128_f32[2]; out[5] = B1A1.m128_f32[1];
	out[6] = B2A2.m128_f32[3]; out[7] = B2A2.m128_f32[2]; out[8] = B2A2.m128_f32[1];

	memcpy((void*)this->a, &Matrix3(
		out[0], out[3], out[6],
		out[1], out[4], out[7],
		out[2], out[5], out[8]), sizeof(float) * 9);

	return *this;
}

Matrix3 Matrix3::operator*=(Matrix3 const& rhs) const
{

	float out[9];

	__m128 A0 = _mm_set_ps(rhs.a[0], rhs.a[3], rhs.a[6], 0);
	__m128 A1 = _mm_set_ps(rhs.a[1], rhs.a[4], rhs.a[7], 0);
	__m128 A2 = _mm_set_ps(rhs.a[2], rhs.a[5], rhs.a[8], 0);

	__m128 B0 = _mm_set_ps(a[0], a[1], a[2], 0);
	__m128 B1 = _mm_set_ps(a[3], a[4], a[5], 0);
	__m128 B2 = _mm_set_ps(a[6], a[7], a[8], 0);

	__m128 B0A0 = _mm_mul_ps(B0, A0); __m128 B0A1 = _mm_mul_ps(B0, A1); __m128 B0A2 = _mm_mul_ps(B0, A2);
	__m128 B1A0 = _mm_mul_ps(B1, A0); __m128 B1A1 = _mm_mul_ps(B1, A1); __m128 B1A2 = _mm_mul_ps(B1, A2);
	__m128 B2A0 = _mm_mul_ps(B2, A0); __m128 B2A1 = _mm_mul_ps(B2, A1); __m128 B2A2 = _mm_mul_ps(B2, A2);

	__m128 HAddB0A0 = _mm_hadd_ps(B0A0, B0A0); __m128 HAddB0A1 = _mm_hadd_ps(B0A1, B0A1); __m128 HAddB0A2 = _mm_hadd_ps(B0A2, B0A2);
	__m128 HAddB1A0 = _mm_hadd_ps(B1A0, B1A0); __m128 HAddB1A1 = _mm_hadd_ps(B1A1, B1A1); __m128 HAddB1A2 = _mm_hadd_ps(B1A2, B1A2);
	__m128 HAddB2A0 = _mm_hadd_ps(B2A0, B2A0); __m128 HAddB2A1 = _mm_hadd_ps(B2A1, B2A1); __m128 HAddB2A2 = _mm_hadd_ps(B2A2, B2A2);

	out[0] = _mm_hadd_ps(HAddB0A0, HAddB0A0).m128_f32[0]; out[1] = _mm_hadd_ps(HAddB0A1, HAddB0A1).m128_f32[0]; out[2] = _mm_hadd_ps(HAddB0A2, HAddB0A2).m128_f32[0];
	out[3] = _mm_hadd_ps(HAddB1A0, HAddB1A0).m128_f32[0]; out[4] = _mm_hadd_ps(HAddB1A1, HAddB1A1).m128_f32[0]; out[5] = _mm_hadd_ps(HAddB1A2, HAddB1A2).m128_f32[0];
	out[6] = _mm_hadd_ps(HAddB2A0, HAddB2A0).m128_f32[0]; out[7] = _mm_hadd_ps(HAddB2A1, HAddB2A1).m128_f32[0]; out[8] = _mm_hadd_ps(HAddB2A2, HAddB2A2).m128_f32[0];

	memcpy((void*)this->a, &Matrix3(
		out[0], out[3], out[6],
		out[1], out[4], out[7],
		out[2], out[5], out[8]), sizeof(float) * 9);

	return *this;
}

Vector3 Matrix3::operator*(Vector3 const& rhs) const
{
	__m128 row1 = _mm_set_ps(a0, a1, a2, 0);
	__m128 row2 = _mm_set_ps(a3, a4, a5, 0);
	__m128 row3 = _mm_set_ps(a6, a7, a8, 0);

	//__m128 vector = _mm_set_ps(rhs.getW(), rhs.getZ(), rhs.getY(), rhs.getX());
	__m128 vector = _mm_set_ps(rhs.getX(), rhs.getY(), rhs.getZ(), 0);

	__m128 mulrow1 = _mm_mul_ps(row1, vector);
	__m128 mulrow2 = _mm_mul_ps(row2, vector);
	__m128 mulrow3 = _mm_mul_ps(row3, vector);

	__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
	sum_01 = _mm_hadd_ps(sum_01, sum_01);
	__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
	sum_02 = _mm_hadd_ps(sum_02, sum_02);
	__m128 sum_03 = _mm_hadd_ps(mulrow3, mulrow3);
	sum_03 = _mm_hadd_ps(sum_03, sum_03);

	Vector3 res(sum_01.m128_f32[0], sum_02.m128_f32[0], sum_03.m128_f32[0]);

	return res;
}

const Vector3 operator*(const Vector3& lhs, const Matrix3& rhs)
{
	__m128 row1 = _mm_set_ps(rhs.a0, rhs.a3, rhs.a6, 0);
	__m128 row2 = _mm_set_ps(rhs.a1, rhs.a4, rhs.a7, 0);
	__m128 row3 = _mm_set_ps(rhs.a2, rhs.a5, rhs.a8, 0);


	__m128 vector = _mm_set_ps(lhs.getX(), lhs.getY(), lhs.getZ(), 0);

	__m128 mulrow1 = _mm_mul_ps(row1, vector);
	__m128 mulrow2 = _mm_mul_ps(row2, vector);
	__m128 mulrow3 = _mm_mul_ps(row3, vector);

	__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
	sum_01 = _mm_hadd_ps(sum_01, sum_01);
	__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
	sum_02 = _mm_hadd_ps(sum_02, sum_02);
	__m128 sum_03 = _mm_hadd_ps(mulrow3, mulrow3);
	sum_03 = _mm_hadd_ps(sum_03, sum_03);

	Vector3 res(sum_01.m128_f32[0], sum_02.m128_f32[0], sum_03.m128_f32[0]);

	return res;
}

Matrix3 Matrix3::operator/(float const& rhs) const
{
	float out[16];
	__m128 multiplicator = _mm_set_ps(rhs, rhs, rhs, rhs);

	__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
	__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
	__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
	__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

	_mm_storeu_ps((float*)&out[0], _mm_div_ps(row0, multiplicator));
	_mm_storeu_ps((float*)&out[4], _mm_div_ps(row1, multiplicator));
	_mm_storeu_ps((float*)&out[8], _mm_div_ps(row2, multiplicator));
	_mm_storeu_ps((float*)&out[12], _mm_div_ps(row3, multiplicator));

	return Matrix3((float*)out);
}

Matrix3 Matrix3::operator*(float const& rhs) const
{
	float out[16];
	__m128 multiplicator = _mm_set_ps(rhs, rhs, rhs, rhs);

	__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
	__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
	__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
	__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

	_mm_storeu_ps((float*)&out[0], _mm_mul_ps(row0, multiplicator));
	_mm_storeu_ps((float*)&out[4], _mm_mul_ps(row1, multiplicator));
	_mm_storeu_ps((float*)&out[8], _mm_mul_ps(row2, multiplicator));
	_mm_storeu_ps((float*)&out[12], _mm_mul_ps(row3, multiplicator));

	return Matrix3((float*)out);
}

float Matrix3::GetDeterminant() const
{
	// http://www32.cplusplus.com/forum/beginner/267880/
	int n = 3;

	float det = 1;

	float tempA[3][3];
	memcpy(tempA, aa, sizeof(float) * 9);
	// Row operations for i = 0, ,,,, n - 2 (n-1 not needed)
	for (int i = 0; i < n - 1; i++)
	{
		// Partial pivot: find row r below with largest element in column i
		int r = i;
		float maxA = abs(tempA[i][i]);
		for (int k = i + 1; k < n; k++)
		{
			float val = abs(tempA[k][i]);
			if (val > maxA)
			{
				r = k;
				maxA = val;
			}
		}
		if (r != i)
		{
			for (int j = i; j < n; j++)
			{
				std::swap(tempA[i][j], tempA[r][j]);
			}
			det = -det;
		}

		// Row operations to make upper-triangular
		float pivot = tempA[i][i];
		if (abs(pivot) < INVISION_MATH_NUM_SMALL) return 0.0;              // Singular matrix

		for (int r = i + 1; r < n; r++)                    // On lower rows
		{
			float multiple = tempA[r][i] / pivot;                // Multiple of row i to clear element in ith column
			for (int j = i; j < n; j++) tempA[r][j] -= multiple * tempA[i][j];
		}
		det *= pivot;                                        // Determinant is product of diagonal
	}

	det *= tempA[n - 1][n - 1];


	return (float)det;
}

Matrix3 Matrix3::GetCoFactor() const
{
	float subMat[3][3];

	subMat[0][0] = 1 * (a[4] * a[8] - a[5] * a[7]);
	subMat[1][0] = -1 * (a[3] * a[8] - a[5] * a[6]);
	subMat[2][0] = 1 * (a[1] * a[5] - a[2] * a[4]);
	subMat[0][1] = -1 * (a[1] * a[8] - a[2] * a[7]);
	subMat[1][1] = 1 * (a[0] * a[8] - a[2] * a[6]);
	subMat[2][1] = -1 * (a[0] * a[5] - a[2] * a[3]);
	subMat[0][2] = 1 * (a[1] * a[5] - a[2] * a[4]);
	subMat[1][2] = -1 * (a[0] * a[5] - a[2] * a[3]);
	subMat[2][2] = 1 * (a[0] * a[4] - a[1] * a[3]);

	return Matrix3(*subMat);
}

Matrix3 Matrix3::Triangulate() const
{
	// http://www32.cplusplus.com/forum/beginner/267880/
	int n = 3;

	float tempA[3][3];
	memcpy(tempA, aa, sizeof(float) * 9);
	// Row operations for i = 0, ,,,, n - 2 (n-1 not needed)
	for (int i = 0; i < n - 1; i++)
	{
		// Partial pivot: find row r below with largest element in column i
		int r = i;
		float maxA = abs(tempA[i][i]);
		for (int k = i + 1; k < n; k++)
		{
			float val = abs(tempA[k][i]);
			if (val > maxA)
			{
				r = k;
				maxA = val;
			}
		}
		if (r != i)
		{
			for (int j = i; j < n; j++)
			{
				std::swap(tempA[i][j], tempA[r][j]);
			}
		}

		// Row operations to make upper-triangular
		float pivot = tempA[i][i];
		if (abs(pivot) < INVISION_MATH_NUM_SMALL) return 0.0;              // Singular matrix

		for (int r = i + 1; r < n; r++)                    // On lower rows
		{
			float multiple = tempA[r][i] / pivot;                // Multiple of row i to clear element in ith column
			for (int j = i; j < n; j++) tempA[r][j] -= multiple * tempA[i][j];
		}
		// Determinant is product of diagonal
	}

	return Matrix3(*tempA);
}

Matrix3 Matrix3::Invert() const
{
	float d = 1 / GetDeterminant();

	Matrix3 coFactorMatrix = GetCoFactor();

	return coFactorMatrix * d;

}

Matrix3 Matrix3::GetTranspose() const
{
	return{ a0, a3, a6,
			a1, a4, a7,
			a2, a5, a8 };
}

Matrix3 Matrix3::Identity()
{
	return{ 1, 0, 0,
				0, 1, 0,
				0, 0, 1 };
}

Matrix3 Matrix3::Translate(const Vector3& v)
{
#ifdef ROWMAJOR
	return{ 1, 0, 0,
			0, 1, 0,
			v.getX(), v.getY(), 1
	};
#else
	return{ 1, 0, v.getX(),
			0, 1, v.getY(),
			0, 0, 1
	};
#endif
}

Matrix3 Matrix3::RotateX(const float f)
{


	float cos = cosf(f * PI_F / 180);
	float sin = sinf(f * PI_F / 180);

#ifdef ROWMAJOR
	return{ 1.0f, 0.0f, 0.0f,
			0.0f, cos, sin,
			0.0f, -sin, cos,
	};
#else
	return{ 1.0f, 0.0f, 0.0f,
			0.0f, cos, -sin,
			0.0f, sin, cos,
	};
#endif
}

Matrix3 Matrix3::RotateY(const float f)
{
	float cos = cosf(f * PI_F / 180);
	float sin = sinf(f * PI_F / 180);


#ifdef ROWMAJOR
	return{ cos, 0.0f, -sin,
			0.0f, 1.0f, 0.0f,
			sin, 0.0f, cos
	};
#else
	return{ cos, 0.0f, sin,
			0.0f, 1.0f, 0.0f,
			-sin, 0.0f, cos
	};
#endif
}

Matrix3 Matrix3::RotateZ(const float f)
{
	float cos = cosf(f * PI_F / 180);
	float sin = sinf(f * PI_F / 180);


#ifdef ROWMAJOR
	return{ cos, sin, 0.0f,
			-sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f
	};
#else
	return{ cos, -sin, 0.0f,
			sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f
	};
#endif
}

Matrix3 Matrix3::RotateXYZ(const float x, const float y, const float z)
{
	return RotateX(x) * RotateY(y) * RotateZ(y);
}

Matrix3 Matrix3::RotateXYZ(const Vector3& v)
{
	return RotateX(v.getX()) * RotateY(v.getY()) * RotateZ(v.getZ());
}

Matrix3 Matrix3::Scale(const Vector3& v)
{
	return{ v.getX(), 0, 0,
			0, v.getY(), 0,
			0, 0, v.getZ() };
}

Matrix3 Matrix3::Scale(const float v)
{
	return{ v, 0, 0,
				0, v, 0,
				0, 0, v };
}