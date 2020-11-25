// Purpose:	  two dimensional Vector class and calculations
// Author:	  Dennis Koehler
// Created:	  2018-03-09
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR2_H
#define VECTOR2_H

#include "base/SIMD.h"
namespace Invision
{

	/**
	* \brief two dimensional vector
	*/
	_MM_ALIGN16 class Vector2
	{
	private:
		float x, y;
		__m128 vec;
	public:

		/**
		* \brief Initialize a two dimensional Vector
		*/
		INVISION_API Vector2() : x(0), y(0), vec(_mm_setzero_ps()) {}

		/**
		* \brief Initialize a two dimensional Vector
		* \param x X Coordinate
		* \param y Y Coordinate
		*/
		INVISION_API Vector2(float x, float y);


		/**
		* \brief Adds a Vector to another Vector
		* \param rhs A Vector object
		* \return A added Vector object
		*/
		INVISION_API Vector2 operator+ (const Vector2 &rhs) const;

		/**
		* \brief Subtracts a Vector to another Vector
		* \param rhs A Vector object
		* \return A subtracted Vector object
		*/
		INVISION_API Vector2 operator- (const Vector2 &rhs) const;

		/**
		* \brief Adds a Vector
		* \param lhs A Vector object
		* \return A added Vector object
		*/
		INVISION_API Vector2 operator+= (const Vector2 &lhs);

		/**
		* \brief Subtracts a Vector
		* \param lhs A Vector object
		* \return A Subtracted Vector object
		*/
		INVISION_API Vector2 operator-= (const Vector2 &lhs);

		/**
		* \brief checks Vectors, whether they are equal
		* \param rhs A Vector object
		* \return true, if both vectors are the same and false, if both vectors are not the same
		*/
		INVISION_API bool operator==(Vector2 const& rhs) const;


		/**
		* \brief calculares the vector product of two vectors
		* \param rhs A vector object
		* \return the result of the vector product calculation
		*/
		INVISION_API float cross(const Vector2 &rhs) const;

		/**
		* \brief multiplies the vector with a scale
		* \param scalar a float value
		* \return the result of the multiplied vector with a scale
		*/
		INVISION_API Vector2 operator*(const float & scalar);

		/**
		* \brief divides the vector with a scale
		* \param scalar a float value
		* \return the result of the divided vector with a scale
		*/
		INVISION_API Vector2 operator/(const float & scalar);

		/**
		* \brief calculates the dot product of a vector
		* \param rhs a Vector object
		* \return the result of the dot product
		*/
		INVISION_API float dot(const Vector2 &rhs) const;

		/**
		* \brief calculates the length of a vector
		* \return the length of the vector
		*/
		INVISION_API float length() const;

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



	};
}
#endif // VECTOR2_H