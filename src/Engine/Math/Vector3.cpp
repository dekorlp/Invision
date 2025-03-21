#include "precompiled.h"

#include "Vector2.h"

#include "Vector3.h"

namespace Invision {
	Vector3::Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->vec = _mm_set_ps(0, z, y, x);
	}

	Vector3::Vector3(Vector2 vec, float z)
	{
		this->x = vec.getX();
		this->y = vec.getY();
		this->z = z;
		this->vec = _mm_set_ps(0, z, y, x);
	}

	Vector3 Vector3::operator+ (const Vector3& rhs) const
	{
		const __m128 res = _mm_add_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1], res.m128_f32[2] };
	}

	Vector3 Vector3::operator- (const Vector3& rhs) const
	{
		const __m128 res = _mm_sub_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1], res.m128_f32[2] };
	}

	Vector3 Vector3::operator-() const
	{
		return{ -x, -y, -z };
	}

	Vector3 Vector3::operator+= (const Vector3& lhs)
	{
		vec = _mm_add_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];

		return *this;
	}

	Vector3 Vector3::operator-= (const Vector3& lhs)
	{
		vec = _mm_sub_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];

		return *this;
	}

	Vector3 Vector3::operator*(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(0, scalar, scalar, scalar);
		vec = _mm_mul_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		return *this;
	}

	Vector3 Vector3::operator/(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(0, scalar, scalar, scalar);
		vec = _mm_div_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		return *this;
	}

	Vector3 operator/(const Vector3& lhs, const float& rhs)
	{
		const __m128 scaleVec = _mm_set_ps(0, rhs, rhs, rhs);
		const __m128 lhsvec = _mm_set_ps(0, lhs.getZ(), lhs.getY(), lhs.getX());
		const __m128 vec = _mm_div_ps(lhsvec, scaleVec);

		return{ vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
	}

	bool Vector3::operator==(Vector3 const& rhs) const
	{
		float compareTolerance = 0.0005f;


		return x >= rhs.x - compareTolerance && x <= rhs.x + compareTolerance
			&& y >= rhs.y - compareTolerance && y <= rhs.y + compareTolerance
			&& z >= rhs.z - compareTolerance && z <= rhs.z + compareTolerance;
	}

	Vector3 Vector3::cross(const Vector3 & rhs) const
	{
		const __m128 res = _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(rhs.vec, rhs.vec, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(rhs.vec, rhs.vec, _MM_SHUFFLE(3, 0, 2, 1)))
		);
		return{ res.m128_f32[0], res.m128_f32[1], res.m128_f32[2] };
	}

	float Vector3::dot(const Vector3 &rhs) const
	{
		__m128 valX = _mm_mul_ps(vec, rhs.vec);
		const __m128 valY = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 1, 1, 1));
		const __m128 valZ = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 2, 2, 2));
		valX = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 0, 0, 0));
		const __m128 res = _mm_add_ps(_mm_add_ps(valX, valY), valZ);
		return res.m128_f32[0];
	}

	float Vector3::length() const
	{
		const __m128 mulVec = _mm_set_ps(0, 2, 2, 2);
		const __m128 pow2Vec = exp_ps(_mm_mul_ps(mulVec, log_ps(vec)));
		const __m128 valX = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 0, 0, 0));
		const __m128 valY = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 1, 1, 1));
		const __m128 valZ = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 2, 2, 2));
		const __m128 res = _mm_add_ps(_mm_add_ps(valX, valY), valZ);
		const __m128 m128len = _mm_sqrt_ps(res);
		const float length = m128len.m128_f32[0];
		return length;
	}

	float Vector3::getZ() const
	{
		return z;
	}

	float Vector3::getY() const
	{
		return y;
	}

	float Vector3::getX() const
	{
		return x;
	}

	float Vector3::getB() const
	{
		return z;
	}

	float Vector3::getG() const
	{
		return y;
	}

	float Vector3::getR() const
	{
		return x;
	}

	void Vector3::SetZ(float val)
	{
		this->z = val;
	}


	void Vector3::SetY(float val)
	{
		this->y = val;
	}


	void Vector3::SetX(float val)
	{
		this->x = val;
	}

	void Vector3::SetR(float val)
	{
		this->x = val;
	}

	void Vector3::SetG(float val)
	{
		this->y = val;
	}

	void Vector3::SetB(float val)
	{
		this->z = val;
	}


	Vector3 Vector3::Normalize(const Vector3& v)
	{
		return v / (sqrtf(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ())); // 0.0000001f added, because of preventing division 0 error
	}

	const Vector3 operator*(const Vector3& lhs, const float& rhs)
	{
		const __m128 scaleVec = _mm_set_ps(0, rhs, rhs, rhs);
		const __m128 lhsvec = _mm_set_ps(0, lhs.getZ(), lhs.getY(), lhs.getX());
		const __m128 vec = _mm_mul_ps(lhsvec, scaleVec);

		return{ vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
	}

	std::size_t Vector3::GetHash() const
	{
		std::size_t h1 = std::hash<float>()(this->getX());
		std::size_t h2 = std::hash<float>()(this->getY());
		std::size_t h3 = std::hash<float>()(this->getZ());
		std::size_t seed = 0;
		seed = h1 ^ (h2 << 1);
		seed = seed ^ (h3 << 1);
		return seed;
	}
}