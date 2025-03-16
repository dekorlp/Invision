#ifndef VECTOR3_H
#define VECTOR3_H

#include "SIMD.h"
#include <cstddef>

class Vector2;

_MM_ALIGN16 class Vector3
{
private:
	union
	{
		__m128 vec;
		struct {
			float x, y, z;
		};
	};
public:

	/**
	 * \brief Initialize a three dimensional Vector
	 */
	Vector3() : x(0), y(0), z(0), vec(_mm_setzero_ps()) {}

	/**
	* \brief Initialize a three dimensional Vector
	* \param x X Coordinate
	* \param y Y Coordinate
	* \param z Z Coordinate
	*/
	Vector3(float x, float y, float z);

	/**
   * \brief Initialize a three dimensional Vector
   * \param x X Coordinate
   * \param y Y Coordinate
   * \param z Z Coordinate
   */
	Vector3(Vector2 vec, float z);


	/**
	 * \brief Adds a Vector to another Vector
	 * \param rhs A Vector object
	 * \return A added Vector object
	 */
	Vector3 operator+ (const Vector3& rhs) const;

	/**
	* \brief Subtracts a Vector to another Vector
	* \param rhs A Vector object
	* \return A subtracted Vector object
	*/
	Vector3 operator- (const Vector3& rhs) const;


	/**
	 * \brief reverse sign
	 * \return vector object
	 */
	Vector3 operator-() const;

	/**
	* \brief Adds a Vector
	* \param lhs A Vector object
	* \return A added Vector object
	*/
	Vector3 operator+= (const Vector3& lhs);

	/**
	* \brief Subtracts a Vector
	* \param lhs A Vector object
	* \return A Subtracted Vector object
	*/
	Vector3 operator-= (const Vector3& lhs);

	/**
	* \brief checks Vectors, whether they are equal
	* \param rhs A Vector object
	* \return true, if both vectors are the same and false, if both vectors are not the same
	*/
	bool operator==(Vector3 const& rhs) const;


	/**
	 * \brief calculares the vector product of two vectors
	 * \param rhs A vector object
	 * \return the result of the vector product calculation
	 */
	Vector3 cross(const Vector3& rhs) const;

	/**
	* \brief multiplies the vector with a scale
	* \param scalar a float value
	* \return the result of the multiplied vector with a scale
	*/
	Vector3 operator*(const float& scalar);

	friend const Vector3 operator*(const Vector3& lhs, const float& rhs);

	/**
	* \brief divides the vector with a scale
	* \param scalar a float value
	* \return the result of the divided vector with a scale
	*/
	Vector3 operator/(const float& scalar);

	/**
	* \brief divides the vector with a scale
	* \param scalar a float value
	* \return the result of the divided vector with a scale
	*/
	friend Vector3 operator/(const Vector3& lhs, const float& rhs);

	/**
	* \brief calculates the dot product of a vector
	* \param rhs a Vector object
	* \return the result of the dot product
	*/
	float dot(const Vector3& rhs) const;

	/**
	* \brief calculates the length of a vector
	* \return the length of the vector
	*/
	float length() const;


	/**
	 * \brief returns Z coordinate
	 * \return float
	 */
	float getZ() const;

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
	* \brief returns Z coordinate
	* \return float
	*/
	float getR() const;

	/**
	* \brief returns Y coordinate
	* \return float
	*/
	float getG() const;

	/**
	* \brief returns X coordinate
	* \return float
	*/
	float getB() const;

	/**
	* \brief set Z coordinate
	*/
	void SetZ(float val);

	/**
	* \brief set Y coordinate
	*/
	void SetY(float val);

	/**
	* \brief set X coordinate
	*/
	void SetX(float val);

	/**
	* \brief set Z coordinate
	*/
	void SetR(float val);

	/**
	* \brief set Y coordinate
	*/
	void SetG(float val);

	/**
	* \brief set X coordinate
	*/
	void SetB(float val);

	/**
	 * \brief normalizes a vector
	 * \param v a vector object
	 * \return a normalized vector
	 */
	static Vector3 Normalize(const Vector3& v);

	/**
   * \brief calculates a hash
	* \return a unsigned long hash value
   */
	std::size_t GetHash() const;


};

#endif