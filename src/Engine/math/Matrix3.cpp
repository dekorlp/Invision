#include "precompiled.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3.h"


namespace Invision {

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

	bool Matrix3::operator==(Matrix3 const & rhs) const
	{
		return rhs.a0 == this->a0 && rhs.a1 == this->a1 && rhs.a2 == this->a2 && rhs.a3 == this->a3
			&& rhs.a4 == this->a4 && rhs.a5 == this->a5 && rhs.a6 == this->a6 && rhs.a7 == this->a7
			&& rhs.a8 == this->a8;
	}

	Matrix3::Matrix3(float *e)
	{
		memcpy(a, e, sizeof(float) * 9);
	}

	Matrix3 Matrix3::operator*(Matrix3 const& rhs) const
	{

		float out[9];

		// Load matrix A into SSE registers
		__m128 A0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 A1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 A2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 A3 = _mm_loadu_ps((const float*)(rhs.a + 12));

		
		
		for (int i = 0; i < 4; i++)
		{

			__m128 x = _mm_set_ps(a[12 + i], a[8 + i], a[4 + i], a[0 + i]);

			__m128 m0 = _mm_mul_ps(A0, x);
			__m128 m1 = _mm_mul_ps(A1, x);
			__m128 m2 = _mm_mul_ps(A2, x);
			__m128 m3 = _mm_mul_ps(A3, x);
			__m128 sum_01 = _mm_hadd_ps(m0, m1);
			__m128 sum_23 = _mm_hadd_ps(m2, m3);
			__m128 result = _mm_hadd_ps(sum_01, sum_23);
			_mm_storeu_ps((float*)&out[i * 4], result);
		}

		

#ifdef ROWMAJOR
		return Matrix3(out[0], out[3], out[6],
			           out[1], out[4], out[7],
			           out[2], out[5], out[8]);

#else
		return Matrix3(out[0], out[1], out[2],
			           out[3], out[4], out[5],
			           out[6], out[7], out[8]);
#endif


	}

	Matrix3 Matrix3::operator+(Matrix3 const& rhs) const
	{
		float out[9];
		__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
		__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
		__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
		__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

		__m128 rowrhs0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 rowrhs1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 rowrhs2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 rowrhs3 = _mm_loadu_ps((const float*)(rhs.a + 12));

		_mm_storeu_ps((float*)&out[0], _mm_add_ps(row0, rowrhs0));
		_mm_storeu_ps((float*)&out[4], _mm_add_ps(row1, rowrhs1));
		_mm_storeu_ps((float*)&out[8], _mm_add_ps(row2, rowrhs2));
		_mm_storeu_ps((float*)&out[12], _mm_add_ps(row3, rowrhs3));

		return Matrix3((float*)out);
	}

	Matrix3 Matrix3::operator-(Matrix3 const& rhs) const
	{
		float out[16];
		__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
		__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
		__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
		__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

		__m128 rowrhs0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 rowrhs1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 rowrhs2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 rowrhs3 = _mm_loadu_ps((const float*)(rhs.a + 12));

		_mm_storeu_ps((float*)&out[0], _mm_sub_ps(row0, rowrhs0));
		_mm_storeu_ps((float*)&out[4], _mm_sub_ps(row1, rowrhs1));
		_mm_storeu_ps((float*)&out[8], _mm_sub_ps(row2, rowrhs2));
		_mm_storeu_ps((float*)&out[12], _mm_sub_ps(row3, rowrhs3));

		return Matrix3((float*)out);
	}

	Matrix3 Matrix3::operator+=(Matrix3 const& rhs) const
	{
		__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
		__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
		__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
		__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

		__m128 rowrhs0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 rowrhs1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 rowrhs2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 rowrhs3 = _mm_loadu_ps((const float*)(rhs.a + 12));

		_mm_storeu_ps((float*)&a[0], _mm_add_ps(row0, rowrhs0));
		_mm_storeu_ps((float*)&a[4], _mm_add_ps(row1, rowrhs1));
		_mm_storeu_ps((float*)&a[8], _mm_add_ps(row2, rowrhs2));
		_mm_storeu_ps((float*)&a[12], _mm_add_ps(row3, rowrhs3));		

		return *this;
	}

	Matrix3 Matrix3::operator-=(Matrix3 const& rhs) const
	{
		__m128 row0 = _mm_loadu_ps((const float*)(a + 0));
		__m128 row1 = _mm_loadu_ps((const float*)(a + 4));
		__m128 row2 = _mm_loadu_ps((const float*)(a + 8));
		__m128 row3 = _mm_loadu_ps((const float*)(a + 12));

		__m128 rowrhs0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 rowrhs1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 rowrhs2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 rowrhs3 = _mm_loadu_ps((const float*)(rhs.a + 12));

		_mm_storeu_ps((float*)&a[0], _mm_sub_ps(row0, rowrhs0));
		_mm_storeu_ps((float*)&a[4], _mm_sub_ps(row1, rowrhs1));
		_mm_storeu_ps((float*)&a[8], _mm_sub_ps(row2, rowrhs2));
		_mm_storeu_ps((float*)&a[12], _mm_sub_ps(row3, rowrhs3));

		return *this;
	}

	Matrix3 Matrix3::operator*=(Matrix3 const& rhs) const
	{

		float out[16];

		__m128 A0 = _mm_loadu_ps((const float*)(rhs.a + 0));
		__m128 A1 = _mm_loadu_ps((const float*)(rhs.a + 4));
		__m128 A2 = _mm_loadu_ps((const float*)(rhs.a + 8));
		__m128 A3 = _mm_loadu_ps((const float*)(rhs.a + 12));


		for (int i = 0; i < 4; i++)
		{

			__m128 x = _mm_set_ps(a[12 + i], a[8 + i], a[4 + i], a[0 + i]);
			__m128 m0 = _mm_mul_ps(A0, x);
			__m128 m1 = _mm_mul_ps(A1, x);
			__m128 m2 = _mm_mul_ps(A2, x);
			__m128 m3 = _mm_mul_ps(A3, x);
			__m128 sum_01 = _mm_hadd_ps(m0, m1);
			__m128 sum_23 = _mm_hadd_ps(m2, m3);
			__m128 result = _mm_hadd_ps(sum_01, sum_23);
			_mm_storeu_ps((float*)&out[i * 4], result);
		}

		

#ifdef ROWMAJOR
		memcpy((void*)this->a, &Matrix3(
			out[0], out[3], out[6],
			out[1], out[4], out[7],
			out[2], out[5], out[8]), sizeof(float) * 9);

#else
		memcpy((void*)this->a, &Matrix3(out[0], out[1], out[2], out[3],
			out[4], out[5], out[6], out[7],
			out[8]), sizeof(float) * 9);		
#endif

		return *this;
	}
		
	Vector3 Matrix3::operator*(Vector3 const& rhs) const
	{
		__m128 row1 = _mm_set_ps(a0, a3, a6, 0);
		__m128 row2 = _mm_set_ps(a1, a4, a7, 0);
		__m128 row3 = _mm_set_ps(a2, a5, a8, 0);
		
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
		int n = 4;

		float det = 1;
		
		float tempA[3][3];
		memcpy(tempA, aa, sizeof(float)*9);
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
		float subVec[3][3];
		float subMat[3][3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// rule of sarrus
				subMat[j][i] = static_cast<float>(pow(-1, i + j) * ((aa[0][0] * aa[1][1] * aa[2][2]) + (aa[0][1] * aa[1][2] * aa[2][0]) + (aa[0][2] * aa[1][0] * aa[2][1])
					- aa[0][2] * aa[1][1] * aa[2][0] - aa[0][0] * aa[1][2] * aa[2][1] - aa[0][1] * aa[1][0] * aa[2][2]));
			}


		}

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
					0, 0, 1};
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


	Matrix3 Matrix3::RotateXY(const float x, const float y)
	{
		return RotateX(x)* RotateY(y);
	}

	Matrix3 Matrix3::RotateXY(const Vector3 &v)
	{
		return RotateX(v.getX()) * RotateY(v.getY());
	}

	Matrix3 Matrix3::Scale(const Vector3 &v)
	{
		return{ v.getX(), 0, 0,
				0, v.getY(), 0,
				0, 0, 1 };
	}
		
	Matrix3 Matrix3::Scale(const float v)
	{
		return{ v, 0, 0,
					0, v, 0,
					0, 0, 1 };
	}
}