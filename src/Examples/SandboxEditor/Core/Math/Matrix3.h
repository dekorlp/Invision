#ifndef MATRIX3_H
#define MATRIX3_H

#include "SIMD.h"

class Vector3;
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
	Matrix3();

	Matrix3(float val);
	/**
	 * \brief initialize a matrix
	 * \param e an array with 16 float values in form of a matrix
	 */
	Matrix3(float* e);

	/**
	 * \brief initialize a matrix
	 * \param e11 - e44 elements of the matrix
	 */
#ifdef ROWMAJOR
	Matrix3(float e11, float e12, float e13,
		float e21, float e22, float e23,
		float e31, float e32, float e33);
#else
	Matrix3(float e11, float e21, float e31,
		float e12, float e22, float e32,
		float e13, float e23, float e33);
#endif

	float& At(int i, int j);

	/**
	* \brief checks Matrices, whether they are equal
	* \param rhs A Matrix object
	* \return true, if both Matrices are the same and false, if both Matrices are not the same
	*/
	bool operator==(Matrix3 const& rhs) const;

	/**
	* \brief multiplies the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the multiplied Matrices
	*/
	Matrix3 operator*(Matrix3 const& rhs) const;

	/**
	* \brief adds the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the added Matrices
	*/
	Matrix3 operator+(Matrix3 const& rhs) const;

	/**
	* \brief subtract the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the subtracted Matrices
	*/
	Matrix3 operator-(Matrix3 const& rhs) const;

	/**
	* \brief Adds a Matrix
	* \param rhs A Matrix object
	* \return A added Matrix object
	*/
	Matrix3 operator+=(Matrix3 const& rhs) const;

	/**
	* \brief Subtract a Matrix
	* \param rhs A Matrix object
	* \return A subtracted Matrix object
	*/
	Matrix3 operator-=(Matrix3 const& rhs) const;

	Matrix3 operator*=(Matrix3 const& rhs) const;

	/**
	* \brief multiplies the Matrix with a scale value
	* \param rhs a float value
	* \return the result of the multiplication with the scale
	*/
	Matrix3 operator*(float const& rhs) const;

	/**
	* \brief calculates the vector transformation
	* \param rhs a vector object
	* \return the result of the multiplication with the vector and the matrix (transformed vector)
	*/
	Vector3 operator*(Vector3 const& rhs) const;

	friend const Vector3 operator*(const Vector3& lhs, const Matrix3& rhs);

	/**
	* \brief divides the Matrix with a scale value
	* \param rhs a float value
	* \return the result of the division with the scale
	*/
	Matrix3 operator/(float const& rhs) const;

	/**
	 * \brief  calculates the determinant from the matrix
	 * \return float determinant
	 */
	float GetDeterminant() const;

	Matrix3 Triangulate() const;

	Matrix3 GetCoFactor() const;

	Matrix3 Invert() const;

	/**
	* \brief  calculates the transponse from the matrix
	* \return the transponse matrix
	*/
	Matrix3 GetTranspose() const;

	/**
	 * \brief returns an identity matrix
	 * \return A Matrix object
	 */
	static Matrix3 Identity();

	/**
	* \brief returns an translated matrix
	* \return A Matrix object
	*/
	static Matrix3 Translate(const Vector3& v);

	/**
	 * \brief rotate around X axis
	 * \param f float value
	 * \return rotated matrix
	 */
	static Matrix3 RotateX(const float f);

	/**
	* \brief rotate around Y axis
	* \param f float value
	* \return rotated matrix
	*/
	static Matrix3 RotateY(const float f);

	/**
	* \brief rotate around Y axis
	* \param f float value
	* \return rotated matrix
	*/
	static Matrix3 RotateZ(const float f);

	/**
	* \brief rotate around XYZ axis
	* \param x float value
	* \param y float value
	* \param z float value
	* \return rotated matrix
	*/
	static Matrix3 RotateXYZ(const float x, const float y, float z);

	/**
	* \brief rotate around XYZ axis
	* \param y vector object
	* \return rotated matrix
	*/
	static Matrix3 RotateXYZ(const Vector3& v);

	/**
	 * \brief use scale transformation
	 * \param v vector object
	 * \return a scaled matrix
	 */
	static Matrix3 Scale(const Vector3& v);

	/**
	 * \brief use scale transformation
	 * \param v float object
	 * \return a scaled matrix
	 */
	static Matrix3 Scale(const float v);
};

#endif