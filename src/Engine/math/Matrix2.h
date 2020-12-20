// Purpose:	  Matrix class and calculations
// Author:	  Dennis Koehler
// Created:	  2018-03-20
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef MATRIX_H
#define MATRIX_H
#include "base/SIMD.h"

namespace Invision
{
	class Vector2;

	/**
	 * \brief Matrix
	 */
	_MM_ALIGN16 class Matrix2
	{
	private:

		union
		{
			struct
			{
				float a0, a1,
					a2, a3;

			};
			float a[4];
			float aa[2][2];
		};

		

	public:
		/**
		 * \brief initialize a matrix
		 */
		INVISION_API Matrix2();

		INVISION_API Matrix2(float val);
		/**
		 * \brief initialize a matrix
		 * \param e an array with 16 float values in form of a matrix
		 */
		INVISION_API Matrix2(float *e);

		/**
		 * \brief initialize a matrix
		 * \param e11 - e44 elements of the matrix
		 */
#ifdef ROWMAJOR
		INVISION_API Matrix2(float e11, float e12
			float e21, float e22);
#else
		INVISION_API Matrix2(float e11, float e21,
		float e12, float e22);
#endif

		INVISION_API float& At(int i, int j);

		/**
		* \brief checks Matrices, whether they are equal
		* \param rhs A Matrix object
		* \return true, if both Matrices are the same and false, if both Matrices are not the same
		*/
		INVISION_API bool operator==(Matrix2 const& rhs) const;

		/**
		* \brief multiplies the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the multiplied Matrices
		*/
		INVISION_API Matrix2 operator*(Matrix2 const& rhs) const;

		/**
		* \brief adds the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the added Matrices
		*/
		INVISION_API Matrix2 operator+(Matrix2 const& rhs) const;

		/**
		* \brief subtract the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the subtracted Matrices
		*/
		INVISION_API Matrix2 operator-(Matrix2 const& rhs) const;

		/**
		* \brief Adds a Matrix
		* \param rhs A Matrix object
		* \return A added Matrix object
		*/
		INVISION_API Matrix2 operator+=(Matrix2 const& rhs) const;

		/**
		* \brief Subtract a Matrix
		* \param rhs A Matrix object
		* \return A subtracted Matrix object
		*/
		INVISION_API Matrix2 operator-=(Matrix2 const& rhs) const;

		INVISION_API Matrix2 operator*=(Matrix2 const& rhs) const;

		/**
		* \brief multiplies the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the multiplication with the scale
		*/
		INVISION_API Matrix2 operator*(float const& rhs) const;

		/**
		* \brief calculates the vector transformation
		* \param rhs a vector object
		* \return the result of the multiplication with the vector and the matrix (transformed vector)
		*/
		INVISION_API Vector2 operator*(Vector2 const& rhs) const;

		INVISION_API friend const Vector2 operator*(const Vector2& lhs, const Matrix2& rhs);

		/**
		* \brief divides the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the division with the scale
		*/
		INVISION_API Matrix2 operator/(float const& rhs) const;

		/**
		 * \brief  calculates the determinant from the matrix
		 * \return float determinant
		 */
		INVISION_API float GetDeterminant() const;

		INVISION_API Matrix2 Triangulate() const;

		INVISION_API Matrix2 GetCoFactor() const;

		INVISION_API Matrix2 Invert() const;

		/**
		* \brief  calculates the transponse from the matrix
		* \return the transponse matrix
		*/
		INVISION_API Matrix2 GetTranspose() const;

		/**
		 * \brief returns an identity matrix
		 * \return A Matrix object
		 */
		INVISION_API static Matrix2 Identity();

	};
}
#endif //MATRIX_H