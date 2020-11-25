#include "precompiled.h"
#include "base/SIMD.h"
#include "Matrix.h"
namespace Invision {

	Matrix::Matrix()
	{

	}

	Matrix::Matrix(float e11, float e12, float e13, float e14, float e21, float e22, float e23, float e24, float e31,
		float e32, float e33, float e34, float e41, float e42, float e43, float e44) : a11(e11), a12(e12), a13(e13), a14(e14),
		a21(e21), a22(e22), a23(e23), a24(e24),
		a31(e31), a32(e32), a33(e33), a34(e34),
		a41(e41), a42(e42), a43(e43), a44(e44)
	{
	}

	Matrix::Matrix(float val) : a11(val), a12(0), a13(0), a14(0),
		a21(0), a22(val), a23(0), a24(0),
		a31(0), a32(0), a33(val), a34(0),
		a41(0), a42(0), a43(0), a44(val)
	{

	}

	Matrix::Matrix(float *e)
	{
		memcpy(a, e, sizeof(float) * 16);
	}

	bool Matrix::operator==(Matrix const & rhs) const
	{
		return rhs.a11 == this->a11 && rhs.a12 == this->a12 && rhs.a13 == this->a13 && rhs.a14 == this->a14
			&& rhs.a21 == this->a21 && rhs.a22 == this->a22 && rhs.a23 == this->a23 && rhs.a24 == this->a24
			&& rhs.a31 == this->a31 && rhs.a32 == this->a32 && rhs.a33 == this->a33 && rhs.a34 == this->a34
			&& rhs.a41 == this->a41 && rhs.a42 == this->a42 && rhs.a43 == this->a43 && rhs.a44 == this->a44;
	}

	Matrix Matrix::operator*(Matrix const& rhs) const
	{
		float out[16];
		// Load matrix A into SSE registers
		__m128 A0 = _mm_loadu_ps((const float*)(a + 0));
		__m128 A1 = _mm_loadu_ps((const float*)(a + 4));
		__m128 A2 = _mm_loadu_ps((const float*)(a + 8));
		__m128 A3 = _mm_loadu_ps((const float*)(a + 12));


		for (int i = 0; i < 4; i++)
		{
			__m128 x = _mm_set_ps(rhs.a[12 + i], rhs.a[8 + i], rhs.a[4 + i], rhs.a[0 + i]);
			__m128 m0 = _mm_mul_ps(A0, x);
			__m128 m1 = _mm_mul_ps(A1, x);
			__m128 m2 = _mm_mul_ps(A2, x);
			__m128 m3 = _mm_mul_ps(A3, x);
			__m128 sum_01 = _mm_hadd_ps(m0, m1);
			__m128 sum_23 = _mm_hadd_ps(m2, m3);
			__m128 result = _mm_hadd_ps(sum_01, sum_23);
			_mm_storeu_ps((float*)&out[i * 4], result);
		}

		return Matrix(out[0], out[4], out[8], out[12],
			out[1], out[5], out[9], out[13],
			out[2], out[6], out[10], out[14],
			out[3], out[7], out[11], out[15]);
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

	Matrix Matrix::operator/(float const& rhs)
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

	Vector3 Matrix::operator*(Vector3 const& rhs) const
	{
		__m128 row1 = _mm_set_ps(a14, a13, a12, a11);
		__m128 row2 = _mm_set_ps(a24, a23, a22, a21);
		__m128 row3 = _mm_set_ps(a34, a33, a32, a31);
		__m128 row4 = _mm_set_ps(a44, a43, a42, a41);

		__m128 vector = _mm_set_ps(0, rhs.getZ(), rhs.getY(), rhs.getX());

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

		float w = sum_04.m128_f32[0];
		Vector3 res(sum_01.m128_f32[0], sum_02.m128_f32[0], sum_03.m128_f32[0]);
		if (w != 1.0f && w != 0.0f) // w / 0 becomes infinity
		{
			res = res / w;
		}

		return res;
	}

	float Matrix::getDeterminant() const
	{
		return{ a11 * (a22 * a33 - a23 * a32) -
			a12 * (a21 * a33 - a23 * a31) +
			a13 * (a21 * a32 - a22 * a31) };
	}

	Matrix Matrix::getTranspose() const
	{
		return{ a11, a21, a31, a41,
				a12, a22, a32, a42,
				a13, a23, a33, a43,
				a14, a24, a34, a44 };
	}

	Matrix Matrix::Identity()
	{
		return{ 1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1 };
	}

	Matrix Matrix::TranslateDX(const Vector3& v)
	{
		return{ 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				v.getX(), v.getY(), v.getZ(), 1 };
	}

	Matrix Matrix::TranslateVK(const Vector3& v)
	{
		return{ 1, 0, 0, v.getX(),
			0, 1, 0, v.getY(),
			0, 0, 1, v.getZ(),
			0, 0, 0, 1 };
	}

	Matrix Matrix::RotateX(const float f)
	{
		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cos, sin, 0.0f,
				0.0f, -sin, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix Matrix::RotateY(const float f)
	{
		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);
		return{ cos, 0.0f, -sin, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sin, 0.0f, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix Matrix::RotateZ(const float f)
	{
		float cos = cosf(f * PI_F / 180);
		float sin = sinf(f * PI_F / 180);
		return{ cos, sin, 0.0f, 0.0f,
				-sin, cos, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix Matrix::RotateXYZ(const float x, const float y, const float z)
	{
		return RotateZ(z) * RotateX(x) * RotateY(y);
	}

	Matrix Matrix::RotateXYZ(const Vector3 &v)
	{
		return RotateZ(v.getZ()) * RotateX(v.getX()) * RotateY(v.getY());
	}

	Matrix Matrix::RotateAxis(const Vector3 &v, const float f)
	{
		const float fSin = sinf(-f * PI_F / 180), fCos = cosf(-f * PI_F / 180);
		const float fOneMinusCos = 1.0f - fCos;

		const Vector3 vAxis = Vector3::Normalize(v);

		return{ (vAxis.getX() * vAxis.getX()) * fOneMinusCos + fCos,
				(vAxis.getX() * vAxis.getY()) * fOneMinusCos - (vAxis.getZ() * fSin),
				(vAxis.getX() * vAxis.getZ()) * fOneMinusCos + (vAxis.getY() * fSin), 0.0f,
				(vAxis.getY() * vAxis.getX()) * fOneMinusCos + (vAxis.getZ() * fSin),
				(vAxis.getY() * vAxis.getY()) * fOneMinusCos + fCos,
				(vAxis.getY() * vAxis.getZ()) * fOneMinusCos - (vAxis.getX() * fSin), 0.0f,
				(vAxis.getZ() * vAxis.getX()) * fOneMinusCos - (vAxis.getY() * fSin),
				(vAxis.getZ() * vAxis.getY()) * fOneMinusCos + (vAxis.getX() * fSin),
				(vAxis.getZ() * vAxis.getZ()) * fOneMinusCos + fCos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
	}

	Matrix Matrix::Scale(const Vector3 &v)
	{
		return{ v.getX(), 0, 0, 0,
				 0, v.getY(), 0, 0,
				 0, 0, v.getZ(), 0,
				 0, 0, 0, 1 };
	}

	Matrix Matrix::Axis(const Vector3 &xAxis,
		const Vector3 &yAxis,
		const Vector3 &zAxis)
	{
		return{ xAxis.getX(), xAxis.getY(), xAxis.getZ(), 0,
				yAxis.getX(), yAxis.getY(), yAxis.getZ(), 0,
				zAxis.getX(), zAxis.getY(), zAxis.getZ(), 0,
				0, 0, 0, 1 };
	}

	Matrix Matrix::CameraDX(const Vector3 &vPos, const Vector3 &vLookAt, const Vector3 &vUp)
	{
		Vector3 vZAxis = Vector3::Normalize(vLookAt - vPos);

		Vector3 vXAxis = Vector3::Normalize(vUp.cross(vZAxis));
		Vector3 vYAxis = Vector3::Normalize(vZAxis.cross(vXAxis));

		return Matrix::TranslateDX(-vPos) * Matrix(vXAxis.getX(), vYAxis.getX(), vZAxis.getX(), 0.0f,
			vXAxis.getY(), vYAxis.getY(), vZAxis.getY(), 0.0f,
			vXAxis.getZ(), vYAxis.getZ(), vZAxis.getZ(), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix Matrix::CameraVK(const Vector3 &vPos, const Vector3 &vLookAt, const Vector3 &vUp)
	{
		Vector3 vZAxis = Vector3::Normalize( vPos - vLookAt); // forward
		Vector3 vXAxis = Vector3::Normalize(vUp.cross(vZAxis)); // left
		Vector3 vYAxis = Vector3::Normalize(vZAxis.cross(vXAxis)); // up

		return
			Matrix(vXAxis.getX(), vYAxis.getX(), vZAxis.getX(), 0,
				vXAxis.getY(), vYAxis.getY(), vZAxis.getY(), 0,
				vXAxis.getZ(), vYAxis.getZ(), vZAxis.getZ(), 0,
			-vXAxis.dot(vPos), -vYAxis.dot(vPos), -vZAxis.dot(vPos), 1.0f) ;
	}

	Matrix Matrix::PerspectiveDX(const float &anglef, const float aspect, const float &nearf, const  float &farf)
	{
		const float scale = 1.0f / tanf(anglef * 0.5f);
		float nf = 1.0f / (nearf - farf);

		return{ scale / aspect, 0.0f, 0.0f, 0.0f,
				0.0f , scale, 0.0f, 0.0f,
				0.0f, 0.0f, (nearf + farf) * nf, (2.0f * nearf * farf) * nf,
				0.0f, 0.0f, -1, 0.0f
		};
	}

	Matrix Matrix::PerspectiveVK(const float &anglef, const float aspect, const float &nearf, const  float &farf)
	{
		float tanCalc = 0.5f * anglef;

		float degrees = tanCalc * 4.0f * atan(1.0f) / 180.0f;

		float f = 1.0f / tan(degrees);

		float range = nearf - farf;
		const float scale = 1.0f / tanf(anglef * 0.5f);
		float nf = 1.0f / (nearf - farf);

		return{ f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f , -f, 0.0f, 0.0f,
			0.0f, 0.0f, farf / (nearf - farf) , -1,
			0.0f, 0.0f, (nearf * farf) / (nearf - farf), 0.0f
		};
	}

	Matrix Matrix::Orthogonal(const float &width, const float &height, const float &nZ, const float &fZ)
	{
		return{ 2.0f / width, 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / height, 0.0f, 0.0f,
				0.0f, 0.0f, -2.0f / (fZ - nZ), 0.0f,
				0.0f, 0.0f, -(fZ + nZ) / (fZ - nZ), 1.0f
		};
	}
}