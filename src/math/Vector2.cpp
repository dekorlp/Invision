#include "precompiled.h"


#include "Vector2.h"
namespace Invision {
	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->vec = _mm_set_ps(0, 0, y, x);
	}

	Vector2 Vector2::operator+ (const Vector2& rhs) const
	{
		const __m128 res = _mm_add_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1] };
	}

	Vector2 Vector2::operator- (const Vector2& rhs) const
	{
		const __m128 res = _mm_sub_ps(vec, rhs.vec);
		return{ res.m128_f32[0], res.m128_f32[1] };
	}

	Vector2 Vector2::operator+= (const Vector2& lhs)
	{
		vec = _mm_add_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];

		return *this;
	}

	Vector2 Vector2::operator-= (const Vector2& lhs)
	{
		vec = _mm_sub_ps(vec, lhs.vec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];

		return *this;
	}

	Vector2 Vector2::operator*(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(0, 0, scalar, scalar);
		vec = _mm_mul_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		return *this;
	}

	Vector2 Vector2::operator/(const float & scalar)
	{
		const __m128 scaleVec = _mm_set_ps(0, 0, scalar, scalar);
		vec = _mm_div_ps(vec, scaleVec);
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		return *this;
	}

	bool Vector2::operator==(Vector2 const& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	float Vector2::cross(const Vector2 & rhs) const
	{

		__m128 res = _mm_mul_ps(vec, _mm_shuffle_ps(rhs.vec, rhs.vec, _MM_SHUFFLE(3, 2, 0, 1)));

		return (res.m128_f32[0] - res.m128_f32[1]);
	}

	float Vector2::dot(const Vector2 &rhs) const
	{
		__m128 valX = _mm_mul_ps(vec, rhs.vec);
		const __m128 valY = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 3, 1, 1));
		valX = _mm_shuffle_ps(valX, valX, _MM_SHUFFLE(3, 3, 0, 0));
		const __m128 res = _mm_add_ps(valX, valY);
		return res.m128_f32[0];
	}

	float Vector2::length() const
	{
		const __m128 mulVec = _mm_set_ps(0, 2, 2, 2);
		const __m128 pow2Vec = exp_ps(_mm_mul_ps(mulVec, log_ps(vec)));
		const __m128 valX = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 0, 0, 0));
		const __m128 valY = _mm_shuffle_ps(pow2Vec, pow2Vec, _MM_SHUFFLE(3, 1, 1, 1));
		const __m128 res = _mm_add_ps(valX, valY);
		const __m128 m128len = _mm_sqrt_ps(res);
		const float length = m128len.m128_f32[0];
		return length;
	}

	float Vector2::getY() const
	{
		return y;
	}

	float Vector2::getX() const
	{
		return x;
	}
}