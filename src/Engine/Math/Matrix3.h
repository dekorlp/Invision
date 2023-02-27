// Purpose:	  Matrix class and calculations
// Author:	  Dennis Koehler
// Created:	  2018-03-20
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef MATRIX3_H
#define MATRIX3_H
#include "base/SIMD.h"

namespace Invision
{
	class Vector2;
	class Vector3;

	/**
	 * \brief Matrix
	 */
	_MM_ALIGN16 class Matrix3
	{
	private:

		union
		{
			struct
			{
				float a0, a1, a2,
					a3, a4, a5,
					a6, a7, a8;

			};
			float a[9];
			float aa[3][3];
		};

		

	public:
		/**
		 * \brief initialize a matrix
		 */
		INVISION_API Matrix3();

		INVISION_API Matrix3(float val);
		/**
		 * \brief initialize a matrix
		 * \param e an array with 16 float values in form of a matrix
		 */
		INVISION_API Matrix3(float *e);

		/**
		 * \brief initialize a matrix
		 * \param e11 - e44 elements of the matrix
		 */
#ifdef ROWMAJOR
		INVISION_API Matrix3(float e11, float e12, float e13,
			float e21, float e22, float e23,
			float e31, float e32, float e33);
#else
		INVISION_API Matrix3(float e11, float e21, float e31, 
		float e12, float e22, float e32, 
		float e13, float e23, float e33);
#endif

		INVISION_API float& At(int i, int j);

		/**
		* \brief checks Matrices, whether they are equal
		* \param rhs A Matrix object
		* \return true, if both Matrices are the same and false, if both Matrices are not the same
		*/
		INVISION_API bool operator==(Matrix3 const& rhs) const;

		/**
		* \brief multiplies the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the multiplied Matrices
		*/
		INVISION_API Matrix3 operator*(Matrix3 const& rhs) const;

		/**
		* \brief adds the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the added Matrices
		*/
		INVISION_API Matrix3 operator+(Matrix3 const& rhs) const;

		/**
		* \brief subtract the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the subtracted Matrices
		*/
		INVISION_API Matrix3 operator-(Matrix3 const& rhs) const;

		/**
		* \brief Adds a Matrix
		* \param rhs A Matrix object
		* \return A added Matrix object
		*/
		INVISION_API Matrix3 operator+=(Matrix3 const& rhs) const;

		/**
		* \brief Subtract a Matrix
		* \param rhs A Matrix object
		* \return A subtracted Matrix object
		*/
		INVISION_API Matrix3 operator-=(Matrix3 const& rhs) const;

		INVISION_API Matrix3 operator*=(Matrix3 const& rhs) const;

		/**
		* \brief multiplies the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the multiplication with the scale
		*/
		INVISION_API Matrix3 operator*(float const& rhs) const;

		/**
		* \brief calculates the vector transformation
		* \param rhs a vector object
		* \return the result of the multiplication with the vector and the matrix (transformed vector)
		*/
		INVISION_API Vector3 operator*(Vector3 const& rhs) const;

		INVISION_API friend const Vector3 operator*(const Vector3& lhs, const Matrix3& rhs);

		/**
		* \brief divides the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the division with the scale
		*/
		INVISION_API Matrix3 operator/(float const& rhs) const;

		/**
		 * \brief  calculates the determinant from the matrix
		 * \return float determinant
		 */
		INVISION_API float GetDeterminant() const;

		INVISION_API Matrix3 Triangulate() const;

		INVISION_API Matrix3 GetCoFactor() const;

		INVISION_API Matrix3 Invert() const;

		/**
		* \brief  calculates the transponse from the matrix
		* \return the transponse matrix
		*/
		INVISION_API Matrix3 GetTranspose() const;

		/**
		 * \brief returns an identity matrix
		 * \return A Matrix object
		 */
		INVISION_API static Matrix3 Identity();

		/**
		* \brief returns an translated matrix
		* \return A Matrix object
		*/
		INVISION_API static Matrix3 Translate(const Vector3& v);

		/**
		 * \brief rotate around X axis
		 * \param f float value
		 * \return rotated matrix
		 */
		INVISION_API static Matrix3 RotateX(const float f);

		/**
		* \brief rotate around Y axis
		* \param f float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix3 RotateY(const float f);

		/**
		* \brief rotate around Y axis
		* \param f float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix3 RotateZ(const float f);

		/**
		* \brief rotate around XYZ axis
		* \param x float value
		* \param y float value
		* \param z float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix3 RotateXYZ(const float x, const float y, float z);

		/**
		* \brief rotate around XYZ axis
		* \param y vector object
		* \return rotated matrix
		*/
		INVISION_API static Matrix3 RotateXYZ(const Vector3 &v);

		/**
		 * \brief use scale transformation
		 * \param v vector object
		 * \return a scaled matrix
		 */
		INVISION_API static Matrix3 Scale(const Vector3 &v);

		/**
		 * \brief use scale transformation
		 * \param v float object
		 * \return a scaled matrix
		 */
		INVISION_API static Matrix3 Scale(const float v);
	};
}
#endif //MATRIX3_H