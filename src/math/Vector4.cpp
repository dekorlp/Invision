#include "precompiled.h"

#include "Vector4.h"

namespace Invision {
	Vector4::Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->vec = _mm_set_ps(w, z, y, x);
	}

	Vector4::Vector4(Vector3 vec, float w)
	{
		this->x = vec.getX();
		this->y = vec.getY();
		this->z = vec.getZ();
		this->w = w;
	}

	Vector4::Vector4(Vector2 vec, float z, float w)
	{
		this->x = vec.getX();
		this->y = vec.getY();
		this->z = z;
		this->w = w;
	}

	Vector4::Vector4(Vector2 vec, Vector2 vec2)
	{
		this->x = vec.getX();
		this->y = vec.getY();
		this->z = vec.getX();
		this->w = vec.getY();
	}

	Vector4 Vector4::operator+ (const Vector4& rhs) const
	{
		const __m128 res = _mm_add_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1], res.m128_f32[2], res.m128_f32[3] };
	}

	Vector4 Vector4::operator- (const Vector4& rhs) const
	{
		const __m128 res = _mm_sub_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1], res.m128_f32[2], res.m128_f32[3] };
	}

	Vector4 Vector4::operator-() const
	{
		return{ -x, -y, -z, -w };
	}

	Vector4 Vector4::operator+= (const Vector4& lhs)
	{
		vec = _mm_add_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = vec.m128_f32[3];

		return *this;
	}

	Vector4 Vector4::operator-= (const Vector4& lhs)
	{
		vec = _mm_sub_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = vec.m128_f32[3];

		return *this;
	}

	Vector4 Vector4::operator*(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(scalar, scalar, scalar, scalar);
		vec = _mm_mul_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = vec.m128_f32[3];
		return *this;
	}

	Vector4 Vector4::operator/(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(scalar, scalar, scalar, scalar);
		vec = _mm_div_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = vec.m128_f32[3];
		return *this;
	}

	Vector4 operator/(const Vector4& lhs, const float& rhs)
	{
		const __m128 scaleVec = _mm_set_ps(rhs, rhs, rhs, rhs);
		const __m128 lhsvec = _mm_set_ps(lhs.getW(), lhs.getZ(), lhs.getY(), lhs.getX());
		const __m128 vec = _mm_div_ps(lhsvec, scaleVec);

		return{ vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2], vec.m128_f32[3] };
	}

	bool Vector4::operator==(Vector4 const& rhs) const
	{
		float compareTolerance = 0.0005f;


		return x >= rhs.x - compareTolerance && x <= rhs.x + compareTolerance
			&& y >= rhs.y - compareTolerance && y <= rhs.y + compareTolerance
			&& z >= rhs.z - compareTolerance && z <= rhs.z + compareTolerance
			&& w >= rhs.w - compareTolerance && w <= rhs.w + compareTolerance;
	}

	float Vector4::dot(const Vector4 &rhs) const
	{
		__m128 valX = _mm_mul_ps(vec, rhs.vec);
		const __m128 valY = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 1, 1, 1));
		const __m128 valZ = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 2, 2, 2));
		const __m128 valW = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 3, 3, 3));
		valX = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 0, 0, 0));
		const __m128 res = _mm_add_ps(_mm_add_ps(_mm_add_ps(valX, valY), valZ), valW);
		return res.m128_f32[0];
	}

	float Vector4::length() const
	{
		const __m128 mulVec = _mm_set_ps(2, 2, 2, 2);
		const __m128 pow2Vec = exp_ps(_mm_mul_ps(mulVec, log_ps(vec)));
		const __m128 valX = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 0, 0, 0));
		const __m128 valY = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 1, 1, 1));
		const __m128 valZ = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 2, 2, 2));
		const __m128 valW = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 3, 3, 3));
		const __m128 res = _mm_add_ps(_mm_add_ps(_mm_add_ps(valX, valY), valZ), valW);
		const __m128 m128len = _mm_sqrt_ps(res);
		const float length = m128len.m128_f32[0];
		return length;
	}

	float Vector4::getW() const
	{
		return w;
	}

	float Vector4::getZ() const
	{
		return z;
	}

	float Vector4::getY() const
	{
		return y;
	}

	float Vector4::getX() const
	{
		return x;
	}

	void Vector4::SetW(float val)
	{
		this->w = val;
	}

	void Vector4::SetZ(float val)
	{
		this->z = val;
	}


	void Vector4::SetY(float val)
	{
		this->y = val;
	}


	void Vector4::SetX(float val)
	{
		this->x = val;
	}


	Vector4 Vector4::Normalize(const Vector4& v)
	{
		return v / (sqrtf(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ() + v.getW() * v.getW())); // 0.0000001f added, because of preventing division 0 error
	}

	const Vector4 operator*(const Vector4& lhs, const float& rhs)
	{
		const __m128 scaleVec = _mm_set_ps(rhs, rhs, rhs, rhs);
		const __m128 lhsvec = _mm_set_ps(lhs.getW(), lhs.getZ(), lhs.getY(), lhs.getX());
		const __m128 vec = _mm_mul_ps(lhsvec, scaleVec);

		return{ vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2], vec.m128_f32[3] };
	}

	std::size_t Vector4::GetHash() const
	{
		std::size_t h1 = std::hash<float>()(this->getX());
		std::size_t h2 = std::hash<float>()(this->getY());
		std::size_t h3 = std::hash<float>()(this->getZ());
		std::size_t h4 = std::hash<float>()(this->getW());
		std::size_t seed = 0;
		seed = h1 ^ (h2 << 1);
		seed = seed ^ (h3 << 1);
		seed = seed ^ (h4 << 1);
		return seed;
	}
}