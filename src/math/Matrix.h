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
#include "Vector3.h"
#include "Vector4.h"
namespace Invision
{
	/**
	 * \brief Matrix
	 */
	_MM_ALIGN16 class Matrix
	{
	private:
		union
		{
			struct
			{
#ifdef ROWMAJOR
				float a11, a22, a13, a14,
					a21, a22, a23, a24,
					a31, a22, a33, a34,
					a41, a22, a43, a44;
#else
				float a11, a12, a13, a14,
					a21, a22, a23, a24,
					a31, a32, a33, a34,
					a41, a42, a43, a44;
#endif
			};
			float a[16];
		};
	public:
		/**
		 * \brief initialize a matrix
		 */
		INVISION_API Matrix();

		INVISION_API Matrix(float val);
		/**
		 * \brief initialize a matrix
		 * \param e an array with 16 float values in form of a matrix
		 */
		INVISION_API Matrix(float *e);

		/**
		 * \brief initialize a matrix
		 * \param e11 - e44 elements of the matrix
		 */
#ifdef ROWMAJOR
		INVISION_API Matrix(float e11, float e12, float e13, float e14,
			float e21, float e22, float e23, float e24,
			float e31, float e32, float e33, float e34,
			float e41, float e42, float e43, float e44);
#else
		INVISION_API Matrix(float e11, float e21, float e31, float e41,
		float e12, float e22, float e32, float e42,
		float e13, float e23, float e33, float e43,
		float e14, float e24, float e34, float e44);
#endif


		/**
		* \brief checks Matrices, whether they are equal
		* \param rhs A Matrix object
		* \return true, if both Matrices are the same and false, if both Matrices are not the same
		*/
		INVISION_API bool operator==(Matrix const& rhs) const;

		/**
		* \brief multiplies the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the multiplied Matrices
		*/
		INVISION_API Matrix operator*(Matrix const& rhs) const;

		/**
		* \brief adds the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the added Matrices
		*/
		INVISION_API Matrix operator+(Matrix const& rhs) const;

		/**
		* \brief subtract the Matrix with another Matrix
		* \param rhs a Matrix object
		* \return the result of the subtracted Matrices
		*/
		INVISION_API Matrix operator-(Matrix const& rhs) const;

		/**
		* \brief Adds a Matrix
		* \param rhs A Matrix object
		* \return A added Matrix object
		*/
		INVISION_API Matrix operator+=(Matrix const& rhs) const;

		/**
		* \brief Subtract a Matrix
		* \param rhs A Matrix object
		* \return A subtracted Matrix object
		*/
		INVISION_API Matrix operator-=(Matrix const& rhs) const;

		/**
		* \brief multiplies the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the multiplication with the scale
		*/
		INVISION_API Matrix operator*(float const& rhs) const;

		/**
		* \brief calculates the vector transformation
		* \param rhs a vector object
		* \return the result of the multiplication with the vector and the matrix (transformed vector)
		*/
		//INVISION_API Vector3 operator*(Vector4 const& rhs) const;

		/**
		* \brief calculates the vector transformation
		* \param rhs a vector object
		* \return the result of the multiplication with the vector and the matrix (transformed vector)
		*/
		INVISION_API Vector4 operator*(Vector4 const& rhs) const;

		INVISION_API friend const Vector4 operator*(const Vector4& lhs, const Matrix& rhs);

		/**
		* \brief divides the Matrix with a scale value
		* \param rhs a float value
		* \return the result of the division with the scale
		*/
		INVISION_API Matrix operator/(float const& rhs);

		/**
		 * \brief  calculates the determinant from the matrix
		 * \return float determinant
		 */
		INVISION_API float getDeterminant() const;

		/**
		* \brief  calculates the transponse from the matrix
		* \return the transponse matrix
		*/
		INVISION_API Matrix getTranspose() const;

		/**
		 * \brief returns an identity matrix
		 * \return A Matrix object
		 */
		INVISION_API static Matrix Identity();

		/**
		* \brief returns an translated matrix
		* \return A Matrix object
		*/
		INVISION_API static Matrix Translate(const Vector3& v);

		/**
		 * \brief rotate around X axis
		 * \param f float value
		 * \return rotated matrix
		 */
		INVISION_API static Matrix RotateX(const float f);

		/**
		* \brief rotate around Y axis
		* \param f float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix RotateY(const float f);

		/**
		* \brief rotate around Z axis
		* \param f float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix RotateZ(const float f);

		/**
		* \brief rotate around XYZ axis
		* \param x float value
		* \param y float value
		* \param z float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix RotateXYZ(const float x, const float y, const float z);

		/**
		* \brief rotate around XYZ axis
		* \param y vector object
		* \return rotated matrix
		*/
		INVISION_API static Matrix RotateXYZ(const Vector3 &v);

		/**
		* \brief rotate around any axis
		* \param v vector object
		* \param f float value
		* \return rotated matrix
		*/
		INVISION_API static Matrix RotateAxis(const Vector3 &v, const float f);


		/**
		 * \brief use scale transformation
		 * \param v vector object
		 * \return a scaled matrix
		 */
		INVISION_API static Matrix Scale(const Vector3 &v);

		/**
		 * \brief use scale transformation
		 * \param v float object
		 * \return a scaled matrix
		 */
		INVISION_API static Matrix Scale(const float v);


		/**
		 * \brief align vector on XYZ axis
		 * \param xAxis vector object
		 * \param yAxis vector object
		 * \param zAxis vector object
		 * \return Matrix object
		 */
		INVISION_API static Matrix Axis(const Vector3 &xAxis, const Vector3 &yAxis, const Vector3 &zAxis);


		/**
		 * \brief use camera transformation
		 * \param vPos position vector
		 * \param vLookAt look at vector
		 * \param vUp up Vector
		 * \return Matrix object
		 */
		INVISION_API static Matrix Camera(const Vector3 &vPos, const Vector3 &vLookAt, const Vector3 &vUp);


		/**
		 * \brief use perspective projection
		 * \param angle
		 * \param aspect radio
		 * \param near near plane
		 * \aram far far plane
		 * \return Matrix object
		 */

		INVISION_API static Matrix Perspective(const float &anglef, const float aspect, const float &nearf, const  float &farf);


		/**
		 * \brief use orthogonal projection
		 * \param width
		 * \param height
		 * \param nZ
		 * \param fZ
		 * \return
		 */
		INVISION_API static Matrix Orthogonal(const float &left, const float &right, const float &bottom, const float &top, const float &nearf, const float &farf);
	};
}
#endif //MATRIX_H