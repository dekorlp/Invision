#include "precompiled.h"
#include "base/SIMD.h"
#include "Matrix.h"


namespace Invision {

	Matrix::Matrix() : 
		a0(1), a1(0), a2(0), a3(0),
		a4(0), a5(1), a6(0), a7(0),
		a8(0), a9(0), a10(1), a11(0),
		a12(0), a13(0), a14(0), a15(1)
	{

	}

	Matrix::Matrix(float val) : 
		a0(val), a1(0), a2(0), a3(0),
		a4(0), a5(val), a6(0), a7(0),
		a8(0), a9(0), a10(val), a11(0),
		a12(0), a13(0), a14(0), a15(val)
	{
		
	}

#ifdef ROWMAJOR
	Matrix::Matrix(float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33) 
		: a0(e00), a1(e01), a2(e02), a3(e03),
		  a4(e10), a5(e11), a6(e12), a7(e13),
		  a8(e20), a9(e21), a10(e22), a11(e23),
		  a12(e30), a13(e31), a14(e32), a15(e33)
	{

	}
#else
	Matrix::Matrix(float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33)
		: a0(e00), a1(e10), a2(e20), a3(e30),
		a4(e01), a5(e11), a6(e21), a7(e31),
		a8(e02), a9(e12), a10(e22), a11(e32),
		a12(e03), a13(e13), a14(e23), a15(e33)
	{

	}
#endif 

	bool Matrix::operator==(Matrix const & rhs) const
	{
		return rhs.a0 == this->a0 && rhs.a1 == this->a1 && rhs.a2 == this->a2 && rhs.a3 == this->a3
			&& rhs.a4 == this->a4 && rhs.a5 == this->a5 && rhs.a6 == this->a6 && rhs.a7 == this->a7
			&& rhs.a8 == this->a8 && rhs.a9 == this->a9 && rhs.a10 == this->a10 && rhs.a11 == this->a11
			&& rhs.a12 == this->a12 && rhs.a13 == this->a13 && rhs.a14 == this->a14 && rhs.a15 == this->a15;
	}

	Matrix::Matrix(float *e)
	{
		memcpy(a, e, sizeof(float) * 16);
	}

	Matrix Matrix::operator*(Matrix const& rhs) const
	{

		float out[16];

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
		return Matrix(out[0], out[4], out[8], out[12],
			out[1], out[5], out[9], out[13],
			out[2], out[6], out[10], out[14],
			out[3], out[7], out[11], out[15]);

#else
		return Matrix(out[0], out[1], out[2], out[3],
			out[4], out[5], out[6], out[7],
			out[8], out[9], out[10], out[11],
			out[12], out[13], out[14], out[15]);
#endif


	}

	Matrix Matrix::operator+(Matrix const& rhs) const
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

		return Matrix((float*)out);
	}

	Matrix Matrix::operator-(Matrix const& rhs) const
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

		return Matrix((float*)out);
	}

	Matrix Matrix::operator+=(Matrix const& rhs) const
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

	Matrix Matrix::operator-=(Matrix const& rhs) const
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

	Matrix Matrix::operator*=(Matrix const& rhs) const
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
		memcpy((void*)this->a, &Matrix(out[0], out[4], out[8], out[12],
			out[1], out[5], out[9], out[13],
			out[2], out[6], out[10], out[14],
			out[3], out[7], out[11], out[15]), sizeof(float) * 16);

#else
		memcpy((void*)this->a, &Matrix(out[0], out[1], out[2], out[3],
			out[4], out[5], out[6], out[7],
			out[8], out[9], out[10], out[11],
			out[12], out[13], out[14], out[15]), sizeof(float) * 16);		
#endif

		return *this;
	}
		
	Vector4 Matrix::operator*(Vector4 const& rhs) const
	{
		__m128 row1 = _mm_set_ps(a0, a4, a8, a12);
		__m128 row2 = _mm_set_ps(a1, a5, a9, a13);
		__m128 row3 = _mm_set_ps(a2, a6, a10, a14);
		__m128 row4 = _mm_set_ps(a3, a7, a11, a15);
		
		//__m128 vector = _mm_set_ps(rhs.getW(), rhs.getZ(), rhs.getY(), rhs.getX());
		__m128 vector = _mm_set_ps(rhs.getX(), rhs.getY(), rhs.getZ(), rhs.getW());

		__m128 mulrow1 = _mm_mul_ps(row1, vector);
		__m128 mulrow2 = _mm_mul_ps(row2, vector);
		__m128 mulrow3 = _mm_mul_ps(row3, vector);
		__m128 mulrow4 = _mm_mul_ps(row4, vector);
		
		__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
		sum_01 = _mm_hadd_ps(sum_01, sum_01);
		__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
		sum_02 = _mm_hadd_ps(sum_02, sum_02);
		__m128 sum_03 = _mm_hadd_ps(mulrow3, mulrow3);
		sum_03 = _mm_hadd_ps(sum_03, sum_03);
		__m128 sum_04 = _mm_hadd_ps(mulrow4, mulrow4);
		sum_04 = _mm_hadd_ps(sum_04, sum_04);
		
		Vector4 res(sum_01.m128_f32[0], sum_02.m128_f32[0], sum_03.m128_f32[0], sum_04.m128_f32[0]);
		
		return res;
	}
		
	const Vector4 operator*(const Vector4& lhs, const Matrix& rhs)
	{
		__m128 row1 = _mm_set_ps(rhs.a0, rhs.a4, rhs.a8, rhs.a12);
		__m128 row2 = _mm_set_ps(rhs.a1, rhs.a5, rhs.a9, rhs.a13);
		__m128 row3 = _mm_set_ps(rhs.a2, rhs.a6, rhs.a10, rhs.a14);
		__m128 row4 = _mm_set_ps(rhs.a3, rhs.a7, rhs.a11, rhs.a15);

		
		__m128 vector = _mm_set_ps(lhs.getX(), lhs.getY(), lhs.getZ(), lhs.getW());
		
		__m128 mulrow1 = _mm_mul_ps(row1, vector);
		__m128 mulrow2 = _mm_mul_ps(row2, vector);
		__m128 mulrow3 = _mm_mul_ps(row3, vector);
		__m128 mulrow4 = _mm_mul_ps(row4, vector);
		
		__m128 sum_01 = _mm_hadd_ps(mulrow1, mulrow1);
		sum_01 = _mm_hadd_ps(sum_01, sum_01);
		__m128 sum_02 = _mm_hadd_ps(mulrow2, mulrow2);
		sum_02 = _mm_hadd_ps(sum_02, sum_02);
		__m128 sum_03 = _mm_hadd_ps(mulrow3, mulrow3);
		sum_03 = _mm_hadd_ps(sum_03, sum_03);
		__m128 sum_04 = _mm_hadd_ps(mulrow4, mulrow4);
		sum_04 = _mm_hadd_ps(sum_04, sum_04);
		
		Vector4 res(sum_01.m128_f32[0], sum_02.m128_f32[0], sum_03.m128_f32[0], sum_04.m128_f32[0]);
		
		return res;
	}

	Matrix Matrix::operator/(float const& rhs) const
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

		return Matrix((float*)out);
	}

	Matrix Matrix::operator*(float const& rhs) const
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
		
		return Matrix((float*)out);
	}
		
	float Matrix::GetDeterminant() const
	{
		// http://www32.cplusplus.com/forum/beginner/267880/
		int n = 4;

		float det = 1;
		
		float tempA[4][4];
		memcpy(tempA, aa, sizeof(float)*16);
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

	Matrix Matrix::GetTranspose() const
	{
		return{ a0, a4, a8, a12,
				a1, a5, a9, a13,
				a2, a6, a10, a14,
				a3, a7, a11, a15 };
	}

	Matrix Matrix::Identity()
	{
		return{ 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
	}

	Matrix Matrix::Translate(const Vector3& v)
	{
#ifdef ROWMAJOR
		return{ 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				v.getX(), v.getY(), v.getZ(), 1
		};
#else
		return{ 1, 0, 0, v.getX(),
			0, 1, 0, v.getY(),
			0, 0, 1, v.getZ(),
			0, 0, 0, 1
		};
#endif
	}

	Matrix Matrix::RotateX(const float f)
	{
		

		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);

#ifdef ROWMAJOR
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cos, sin, 0.0f,
				0.0f, -sin, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#else
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cos, -sin, 0.0f,
				0.0f, sin, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#endif
	}

	Matrix Matrix::RotateY(const float f)
	{
		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);


#ifdef ROWMAJOR
		return{ cos, 0.0f, -sin, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sin, 0.0f, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#else
		return{ cos, 0.0f, sin, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				-sin, 0.0f, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#endif
	}

	Matrix Matrix::RotateZ(const float f)
	{
		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);

#ifdef ROWMAJOR
		return{ cos, sin, 0.0f, 0.0f,
				-sin, cos, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#else
		return{ cos, -sin, 0.0f, 0.0f,
				sin, cos, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
#endif

	}

	Matrix Matrix::RotateXYZ(const float x, const float y, const float z)
	{
		return RotateX(x) * RotateZ(z) * RotateY(y);
	}

	Matrix Matrix::RotateXYZ(const Vector3 &v)
	{
		return RotateZ(v.getZ()) * RotateX(v.getX()) * RotateY(v.getY());
	}

	Matrix Matrix::Scale(const Vector3 &v)
	{
		return{ v.getX(), 0, 0, 0,
					0, v.getY(), 0, 0,
					0, 0, v.getZ(), 0,
					0, 0, 0, 1 };
	}
		
	Matrix Matrix::Scale(const float v)
	{
		return{ v, 0, 0, 0,
					0, v, 0, 0,
					0, 0, v, 0,
					0, 0, 0, 1 };
	}

	Matrix Matrix::Camera(const Vector3 &vPos, const Vector3 &vLookAt, const Vector3 &vUp)
	{
		Vector3 vZAxis = Vector3::Normalize(vPos - vLookAt); // forward
		Vector3 vXAxis = Vector3::Normalize(vUp.cross(vZAxis)); // left
		Vector3 vYAxis = Vector3::Normalize(vZAxis.cross(vXAxis)); // up
#ifdef ROWMAJOR
		

		return
			Matrix(vXAxis.getX(), vYAxis.getX(), vZAxis.getX(), 0,
				vXAxis.getY(), vYAxis.getY(), vZAxis.getY(), 0,
				vXAxis.getZ(), vYAxis.getZ(), vZAxis.getZ(), 0,
				-vXAxis.dot(vPos), -vYAxis.dot(vPos), -vZAxis.dot(vPos), 1.0f);
#else
		return
			Matrix(vXAxis.getX(), vXAxis.getY(), vXAxis.getZ(), -vXAxis.dot(vPos),
				vYAxis.getX(), vYAxis.getY(), vYAxis.getZ(), -vYAxis.dot(vPos),
				vZAxis.getX(), vZAxis.getY(), vZAxis.getZ(), -vZAxis.dot(vPos),
				0, 0, 0, 1.0f);
#endif 
	}

	Matrix Matrix::Perspective(const float &anglef, const float aspect, const float &nearf, const  float &farf, bool flipY)
	{
		float tanCalc = 0.5f * anglef;
		float degrees = tanCalc * 4.0f * atan(1.0f) / 180.0f;
		float f = 1.0f / tan(degrees);
		float y = f;
		if (flipY) y *= -1;


#ifdef ROWMAJOR
		
		return{ f / aspect, 0.0f, 0.0f, 0.0f,
			   0.0f , y, 0.0f, 0.0f,
			   0.0f, 0.0f,  farf / (nearf - farf) , -1,
			   0.0f, 0.0f, (nearf * farf) / (nearf - farf), 0.0f
		};
#else 
		return{ f / aspect, 0.0f, 0.0f, 0.0f,
				 0.0f , y, 0.0f, 0.0f,
				0.0f, 0.0f, farf / (nearf - farf) , (nearf * farf) / (nearf - farf),
				0.0f, 0.0f, -1, 0.0f
		};

#endif
	}

	Matrix Matrix::Orthographic(const float &left, const float &right, const float &bottom, const float &top, const float &nearf, const float &farf)
	{
#ifdef ROWMAJOR
		return{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / (bottom - top), 0.0f, 0.0f,
				0.0f, 0.0f, 2.0f / (nearf - farf), 0.0f,
				-(right + left) / (right - left), -(bottom + top) / (bottom - top), nearf / (nearf - farf), 1.0f
		};
#else
		return{ 2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
				0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
				0.0f, 0.0f, -2.0f / (farf - nearf),-(farf + nearf) / (farf - nearf) ,
				0.0f, 0.0f, 0.0f, 1.0f
		};
		
#endif
	}

	Matrix Matrix::RotateAxis(const Vector3 &v, const float f)
	{
		const float fSin = sinf(-f * PI_F / 180), fCos = cosf(-f * PI_F / 180);
		const float fOneMinusCos = 1.0f - fCos;

		const Vector3 vAxis = Vector3::Normalize(v);
#ifdef ROWMAJOR
		return{ (vAxis.getX() * vAxis.getX()) * fOneMinusCos + fCos, (vAxis.getY() * vAxis.getX()) * fOneMinusCos + (vAxis.getZ() * fSin), (vAxis.getZ() * vAxis.getX()) * (vAxis.getZ() * vAxis.getX()) * fOneMinusCos - (vAxis.getY() * fSin), 0.0f,
				(vAxis.getX() * vAxis.getY()) * fOneMinusCos - (vAxis.getZ() * fSin),	(vAxis.getY() * vAxis.getY()) * fOneMinusCos + fCos, (vAxis.getZ() * vAxis.getY()) * fOneMinusCos + (vAxis.getX() * fSin), 0.0f,
				(vAxis.getX() * vAxis.getZ()) * fOneMinusCos + (vAxis.getY() * fSin),	(vAxis.getY() * vAxis.getZ()) * fOneMinusCos - (vAxis.getX() * fSin), (vAxis.getZ() * vAxis.getZ()) * fOneMinusCos + fCos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
	};
#else
		return{ (vAxis.getX() * vAxis.getX()) * fOneMinusCos + fCos, (vAxis.getX() * vAxis.getY()) * fOneMinusCos - (vAxis.getZ() * fSin), (vAxis.getX() * vAxis.getZ()) * fOneMinusCos + (vAxis.getY() * fSin), 0.0f,
				(vAxis.getY() * vAxis.getX()) * fOneMinusCos + (vAxis.getZ() * fSin),	(vAxis.getY() * vAxis.getY()) * fOneMinusCos + fCos, (vAxis.getY() * vAxis.getZ()) * fOneMinusCos - (vAxis.getX() * fSin), 0.0f,
				(vAxis.getZ() * vAxis.getX()) * fOneMinusCos - (vAxis.getY() * fSin),	(vAxis.getZ() * vAxis.getY()) * fOneMinusCos + (vAxis.getX() * fSin), (vAxis.getZ() * vAxis.getZ()) * fOneMinusCos + fCos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
	};
#endif

	}

	Matrix Matrix::Axis(const Vector3 &xAxis,
	const Vector3 &yAxis,
	const Vector3 &zAxis)
	{
#ifdef ROWMAJOR
		return{ xAxis.getX(), xAxis.getY(), xAxis.getZ(), 0,
				yAxis.getX(), yAxis.getY(), yAxis.getZ(), 0,
				zAxis.getX(), zAxis.getY(), zAxis.getZ(), 0,
				0, 0, 0, 1 };
#else
		return{ xAxis.getX(), yAxis.getX(), zAxis.getX(), 0,
				xAxis.getY(), yAxis.getY(), zAxis.getY(), 0,
				xAxis.getZ(), yAxis.getZ(), zAxis.getZ(), 0,
				0, 0, 0, 1 };

#endif
	}
}