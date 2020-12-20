#include "precompiled.h"
#include "Vector2.h"
#include "Matrix2.h"


namespace Invision {

	Matrix2::Matrix2() :
		a0(1), a1(0),
		a2(0), a3(1)
	{

	}

	Matrix2::Matrix2(float val) :
		a0(val), a1(0),
		a2(0), a3(val)
	{
		
	}

#ifdef ROWMAJOR
	Matrix2::Matrix2(float e00, float e01,
					 float e10, float e11) 
		: a0(e00), a1(e01),
		  a2(e10), a3(e11)
	{

	}
#else
	Matrix2::Matrix2(float e00, float e01,
		float e10, float e11)
		: a0(e00), a2(e10), 
		a1(e01), a3(e11)
	{

	}
#endif 

	float& Matrix2::At(int i, int j)
	{
		return aa[i][j];
	}

	bool Matrix2::operator==(Matrix2 const & rhs) const
	{
		return rhs.a0 == this->a0 && rhs.a1 == this->a1 && rhs.a2 == this->a2 && rhs.a3 == this->a3;
	}

	Matrix2::Matrix2(float *e)
	{
		memcpy(a, e, sizeof(float) * 4);
	}

	Matrix2 Matrix2::operator*(Matrix2 const& rhs) const
	{

		float out[4];

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
		return Matrix(out[0], out[2]
			out[1], out[3]);

#else
		return Matrix2(out[0], out[1],
			out[2], out[3]);
#endif


	}

	Matrix2 Matrix2::operator+(Matrix2 const& rhs) const
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

		_mm_storeu_ps((float*)&out[0], _mm_add_ps(row0, rowrhs0));
		_mm_storeu_ps((float*)&out[4], _mm_add_ps(row1, rowrhs1));
		_mm_storeu_ps((float*)&out[8], _mm_add_ps(row2, rowrhs2));
		_mm_storeu_ps((float*)&out[12], _mm_add_ps(row3, rowrhs3));

		return Matrix2((float*)out);
	}

	Matrix2 Matrix2::operator-(Matrix2 const& rhs) const
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

		return Matrix2((float*)out);
	}

	Matrix2 Matrix2::operator+=(Matrix2 const& rhs) const
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

	Matrix2 Matrix2::operator-=(Matrix2 const& rhs) const
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

	Matrix2 Matrix2::operator*=(Matrix2 const& rhs) const
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
		memcpy((void*)this->a, &Matrix(out[0], out[2],
			out[1], out[3]), sizeof(float) * 4);

#else
		memcpy((void*)this->a, &Matrix2(out[0], out[1],
			out[2], out[3]), sizeof(float) * 4);		
#endif

		return *this;
	}
		
	Vector2 Matrix2::operator*(Vector2 const& rhs) const
	{
		__m128 row1 = _mm_set_ps(a0, a2, 0, 0);
		__m128 row2 = _mm_set_ps(a1, a3, 0, 0);
		
		//__m128 vector = _mm_set_ps(rhs.getW(), rhs.getZ(), rhs.getY(), rhs.getX());
		__m128 vector = _mm_set_ps(rhs.getX(), rhs.getY(), 0, 0);

		__m128 mulrow1 = _mm_mul_ps(row1, vector);
		__m128 mulrow2 = _mm_mul_ps(row2, vector);
		
		__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
		sum_01 = _mm_hadd_ps(sum_01, sum_01);
		__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
		sum_02 = _mm_hadd_ps(sum_02, sum_02);

		
		Vector2 res(sum_01.m128_f32[0], sum_02.m128_f32[0]);
		
		return res;
	}
		
	const Vector2 operator*(const Vector2& lhs, const Matrix2& rhs)
	{
		__m128 row1 = _mm_set_ps(rhs.a0, rhs.a2, 0, 0);
		__m128 row2 = _mm_set_ps(rhs.a1, rhs.a3, 0, 0);

		
		__m128 vector = _mm_set_ps(lhs.getX(), lhs.getY(), 0, 0);
		
		__m128 mulrow1 = _mm_mul_ps(row1, vector);
		__m128 mulrow2 = _mm_mul_ps(row2, vector);
		
		__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
		sum_01 = _mm_hadd_ps(sum_01, sum_01);
		__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
		sum_02 = _mm_hadd_ps(sum_02, sum_02);
		
		Vector2 res(sum_01.m128_f32[0], sum_02.m128_f32[0]);
		
		return res;
	}

	Matrix2 Matrix2::operator/(float const& rhs) const
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

		return Matrix2((float*)out);
	}

	Matrix2 Matrix2::operator*(float const& rhs) const
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
		
		return Matrix2((float*)out);
	}
		
	float Matrix2::GetDeterminant() const
	{
		// http://www32.cplusplus.com/forum/beginner/267880/
		int n = 2;

		float det = 1;
		
		float tempA[2][2];
		memcpy(tempA, aa, sizeof(float)*4);
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

	Matrix2 Matrix2::GetCoFactor() const
	{
		float subMat[2][2];
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
			

				// rule of sarrus
				subMat[j][i] = 0;
			}


		}

		return Matrix2(*subMat);
	}

	Matrix2 Matrix2::Triangulate() const
	{
		// http://www32.cplusplus.com/forum/beginner/267880/
		int n = 2;

		float tempA[2][2];
		memcpy(tempA, aa, sizeof(float) * 4);
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

		return Matrix2(*tempA);
	}

	Matrix2 Matrix2::Invert() const
	{
		float d = 1 / GetDeterminant();

		Matrix2 coFactorMatrix = GetCoFactor();

		return coFactorMatrix * d;

	}

	Matrix2 Matrix2::GetTranspose() const
	{
		return{ a0, a2,
				a1, a3};
	}

	Matrix2 Matrix2::Identity()
	{
		return{ 1, 0,
					0, 1 };
	}

	Matrix2 Matrix2::Translate(const Vector2& v)
	{
#ifdef ROWMAJOR
		return{ 1, 0,
				v.getX(), v.getY()
		};
#else
		return{ 1, v.getX(),
			0, v.getY()
		};
#endif
	}

	Matrix2 Matrix2::Scale(const Vector2 &v)
	{
		return{ v.getX(), 0,
					0, v.getY()};
	}
		
	Matrix2 Matrix2::Scale(const float v)
	{
		return{ v, 0,
				0, 1};
	}
}