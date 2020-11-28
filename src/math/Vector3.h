// Purpose:	  three dimensional Vector class and calculations
// Author:	  Dennis Koehler
// Created:	  2018-03-09
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR3_H
#define VECTOR3_H

#include "base/SIMD.h"

namespace Invision
{


	/**
	 * \brief three dimensional vector
	 */
	_MM_ALIGN16 class Vector3
	{
	private:
		
		__m128 vec;
		float x, y, z;
	public:

		/**
		 * \brief Initialize a three dimensional Vector
		 */
		INVISION_API Vector3() : x(0), y(0), z(0), vec(_mm_setzero_ps()) {}

		/**
		* \brief Initialize a three dimensional Vector
		* \param x X Coordinate
		* \param y Y Coordinate
		* \param z Z Coordinate
		*/
		 INVISION_API Vector3(float x, float y, float z);


		/**
		 * \brief Adds a Vector to another Vector
		 * \param rhs A Vector object
		 * \return A added Vector object
		 */
		 INVISION_API Vector3 operator+ (const Vector3 &rhs) const;

		/**
		* \brief Subtracts a Vector to another Vector
		* \param rhs A Vector object
		* \return A subtracted Vector object
		*/
		 INVISION_API Vector3 operator- (const Vector3 &rhs) const;


		/**
		 * \brief reverse sign
		 * \return vector object
		 */
		 INVISION_API Vector3 operator-() const;

		/**
		* \brief Adds a Vector
		* \param lhs A Vector object
		* \return A added Vector object
		*/
		 INVISION_API Vector3 operator+= (const Vector3 &lhs);

		/**
		* \brief Subtracts a Vector
		* \param lhs A Vector object
		* \return A Subtracted Vector object
		*/
		 INVISION_API Vector3 operator-= (const Vector3 &lhs);

		/**
		* \brief checks Vectors, whether they are equal
		* \param rhs A Vector object
		* \return true, if both vectors are the same and false, if both vectors are not the same
		*/
		 INVISION_API bool operator==(Vector3 const& rhs) const;


		/**
		 * \brief calculares the vector product of two vectors
		 * \param rhs A vector object
		 * \return the result of the vector product calculation
		 */
		 INVISION_API Vector3 cross(const Vector3 &rhs) const;

		/**
		* \brief multiplies the vector with a scale
		* \param scalar a float value
		* \return the result of the multiplied vector with a scale
		*/
		 INVISION_API Vector3 operator*(const float & scalar);

		 INVISION_API friend const Vector3 operator*(const Vector3& lhs, const float& rhs);

		/**
		* \brief divides the vector with a scale
		* \param scalar a float value
		* \return the result of the divided vector with a scale
		*/
		 INVISION_API Vector3 operator/(const float & scalar);

		/**
		* \brief divides the vector with a scale
		* \param scalar a float value
		* \return the result of the divided vector with a scale
		*/
		 INVISION_API friend Vector3 operator/(const Vector3& lhs, const float& rhs);

		/**
		* \brief calculates the dot product of a vector
		* \param rhs a Vector object
		* \return the result of the dot product
		*/
		 INVISION_API float dot(const Vector3 &rhs) const;

		/**
		* \brief calculates the length of a vector
		* \return the length of the vector
		*/
		 INVISION_API float length() const;


		/**
		 * \brief returns Z coordinate
		 * \return float
		 */
		 INVISION_API float getZ() const;

		/**
		* \brief returns Y coordinate
		* \return float
		*/
		 INVISION_API float getY() const;

		/**
		* \brief returns X coordinate
		* \return float
		*/
		 INVISION_API float getX() const;

		 /**
		 * \brief returns Z coordinate
		 * \return float
		 */
		 INVISION_API float getR() const;

		 /**
		 * \brief returns Y coordinate
		 * \return float
		 */
		 INVISION_API float getG() const;

		 /**
		 * \brief returns X coordinate
		 * \return float
		 */
		 INVISION_API float getB() const;

		 /**
		 * \brief set Z coordinate
		 */
		 INVISION_API void SetZ(float val);

		 /**
		 * \brief set Y coordinate
		 */
		 INVISION_API void SetY(float val);

		 /**
		 * \brief set X coordinate
		 */
		 INVISION_API void SetX(float val);

		 /**
		 * \brief set Z coordinate
		 */
		 INVISION_API void SetR(float val);

		 /**
		 * \brief set Y coordinate
		 */
		 INVISION_API void SetG(float val);

		 /**
		 * \brief set X coordinate
		 */
		 INVISION_API void SetB(float val);

		/**
		 * \brief normalizes a vector
		 * \param v a vector object
		 * \return a normalized vector
		 */
		 INVISION_API static Vector3 Normalize(const Vector3& v);

		 /**
		* \brief calculates a hash
		 * \return a unsigned long hash value
		*/
		 INVISION_API std::size_t GetHash() const;


	};

}
#endif // VECTOR3_H