#ifndef VECTOR4_H
#define VECTOR4_H


#include "SIMD.h"
#include <cstddef>

class Vector2;
class Vector3;

_MM_ALIGN16 class Vector4
{
private:
	union
	{
		__m128 vec;
		struct {
			float x, y, z, w;
		};
	};
public:

	/**
	 * \brief Initialize a three dimensional Vector
	 */
	Vector4() : x(0), y(0), z(0), w(0), vec(_mm_setzero_ps()) {}

	/**
	* \brief Initialize a four dimensional Vector
	* \param x X Coordinate
	* \param y Y Coordinate
	* \param z Z Coordinate
	*/
	Vector4(float x, float y, float z, float w);

	/**
   * \brief Initialize a four dimensional Vector
   * \param x X Coordinate
   * \param y Y Coordinate
   * \param z Z Coordinate
   */
	Vector4(Vector3 vec, float w);

	/**
  * \brief Initialize a four dimensional Vector
  * \param x X Coordinate
  * \param y Y Coordinate
  * \param z Z Coordinate
  */
	Vector4(Vector2 vec, float z, float w);

	/**
* \brief Initialize a four dimensional Vector
* \param x X Coordinate
* \param y Y Coordinate
* \param z Z Coordinate
*/
	Vector4(Vector2 left, Vector2 right);


	/**
	 * \brief Adds a Vector to another Vector
	 * \param rhs A Vector object
	 * \return A added Vector object
	 */
	Vector4 operator+ (const Vector4& rhs) const;

	/**
	* \brief Subtracts a Vector to another Vector
	* \param rhs A Vector object
	* \return A subtracted Vector object
	*/
	Vector4 operator- (const Vector4& rhs) const;


	/**
	 * \brief reverse sign
	 * \return vector object
	 */
	Vector4 operator-() const;

	/**
	* \brief Adds a Vector
	* \param lhs A Vector object
	* \return A added Vector object
	*/
	Vector4 operator+= (const Vector4& lhs);

	/**
	* \brief Subtracts a Vector
	* \param lhs A Vector object
	* \return A Subtracted Vector object
	*/
	Vector4 operator-= (const Vector4& lhs);

	/**
	* \brief checks Vectors, whether they are equal
	* \param rhs A Vector object
	* \return true, if both vectors are the same and false, if both vectors are not the same
	*/
	bool operator==(Vector4 const& rhs) const;


	/**
	* \brief multiplies the vector with a scale
	* \param scalar a float value
	* \return the result of the multiplied vector with a scale
	*/
	Vector4 operator*(const float& scalar);

	friend const Vector4 operator*(const Vector4& lhs, const float& rhs);

	/**
	* \brief divides the vector with a scale
	* \param scalar a float value
	* \return the result of the divided vector with a scale
	*/
	Vector4 operator/(const float& scalar);

	/**
	* \brief divides the vector with a scale
	* \param scalar a float value
	* \return the result of the divided vector with a scale
	*/
	friend Vector4 operator/(const Vector4& lhs, const float& rhs);

	/**
	* \brief calculates the dot product of a vector
	* \param rhs a Vector object
	* \return the result of the dot product
	*/
	float dot(const Vector4& rhs) const;

	/**
	* \brief calculates the length of a vector
	* \return the length of the vector
	*/
	float length() const;

	/**
	* \brief returns W coordinate
	* \return float
	*/
	float getW() const;

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
   * \brief set W coordinate
   */
	void SetW(float val);

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
	 * \brief normalizes a vector
	 * \param v a vector object
	 * \return a normalized vector
	 */
	static Vector4 Normalize(const Vector4& v);

	/**
   * \brief calculates a hash
	* \return a unsigned long hash value
   */
	std::size_t GetHash() const;


};

#endif