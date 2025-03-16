#ifndef VECTOR2_H
#define VECTOR2_H
#include "SIMD.h"
#include <cstddef>

_MM_ALIGN16 class Vector2
{
private:
	union
	{
		struct {
			float x, y;
		};
		__m128 vec;
	};
public:

	/**
	* \brief Initialize a two dimensional Vector
	*/
	Vector2() : x(0), y(0), vec(_mm_setzero_ps()) {}

	/**
	* \brief Initialize a two dimensional Vector
	* \param x X Coordinate
	* \param y Y Coordinate
	*/
	Vector2(float x, float y);


	/**
	* \brief Adds a Vector to another Vector
	* \param rhs A Vector object
	* \return A added Vector object
	*/
	Vector2 operator+ (const Vector2& rhs) const;

	/**
	* \brief Subtracts a Vector to another Vector
	* \param rhs A Vector object
	* \return A subtracted Vector object
	*/
	Vector2 operator- (const Vector2& rhs) const;

	/**
	* \brief Adds a Vector
	* \param lhs A Vector object
	* \return A added Vector object
	*/
	Vector2 operator+= (const Vector2& lhs);

	/**
	* \brief Subtracts a Vector
	* \param lhs A Vector object
	* \return A Subtracted Vector object
	*/
	Vector2 operator-= (const Vector2& lhs);

	/**
	* \brief checks Vectors, whether they are equal
	* \param rhs A Vector object
	* \return true, if both vectors are the same and false, if both vectors are not the same
	*/
	bool operator==(Vector2 const& rhs) const;


	/**
	* \brief calculares the vector product of two vectors
	* \param rhs A vector object
	* \return the result of the vector product calculation
	*/
	float cross(const Vector2& rhs) const;

	/**
	* \brief multiplies the vector with a scale
	* \param scalar a float value
	* \return the result of the multiplied vector with a scale
	*/
	Vector2 operator*(const float& scalar);

	/**
	* \brief divides the vector with a scale
	* \param scalar a float value
	* \return the result of the divided vector with a scale
	*/
	Vector2 operator/(const float& scalar);

	/**
	* \brief calculates the dot product of a vector
	* \param rhs a Vector object
	* \return the result of the dot product
	*/
	float dot(const Vector2& rhs) const;

	/**
	* \brief calculates the length of a vector
	* \return the length of the vector
	*/
	float length() const;

	/**
	* \brief returns Y coordinate
	* \return float
	*/
	float getY() const;

	/**
	* \brief returns X coordinate
	* \return float
	*/
	float getX() const;

	/**
	* \brief set Y coordinate
	*/
	void SetY(float val);

	/**
	* \brief set X coordinate
	*/
	void SetX(float val);


	/**
   * \brief calculates a hash
	* \return a unsigned long hash value
   */
	std::size_t GetHash() const;
};

#endif