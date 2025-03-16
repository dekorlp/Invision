#ifndef MATRIX2_H
#define MATRIX2_H

#include "SIMD.h"

class Vector2;
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
	Matrix2();

	Matrix2(float val);
	/**
	 * \brief initialize a matrix
	 * \param e an array with 16 float values in form of a matrix
	 */
	Matrix2(float* e);

	/**
	 * \brief initialize a matrix
	 * \param e11 - e44 elements of the matrix
	 */
#ifdef ROWMAJOR
	Matrix2(float e11, float e12,
		float e21, float e22);
#else
	Matrix2(float e11, float e21,
		float e12, float e22);
#endif

	float& At(int i, int j);

	/**
	* \brief checks Matrices, whether they are equal
	* \param rhs A Matrix object
	* \return true, if both Matrices are the same and false, if both Matrices are not the same
	*/
	bool operator==(Matrix2 const& rhs) const;

	/**
	* \brief multiplies the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the multiplied Matrices
	*/
	Matrix2 operator*(Matrix2 const& rhs) const;

	/**
	* \brief adds the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the added Matrices
	*/
	Matrix2 operator+(Matrix2 const& rhs) const;

	/**
	* \brief subtract the Matrix with another Matrix
	* \param rhs a Matrix object
	* \return the result of the subtracted Matrices
	*/
	Matrix2 operator-(Matrix2 const& rhs) const;

	/**
	* \brief Adds a Matrix
	* \param rhs A Matrix object
	* \return A added Matrix object
	*/
	Matrix2 operator+=(Matrix2 const& rhs) const;

	/**
	* \brief Subtract a Matrix
	* \param rhs A Matrix object
	* \return A subtracted Matrix object
	*/
	Matrix2 operator-=(Matrix2 const& rhs) const;

	Matrix2 operator*=(Matrix2 const& rhs) const;

	/**
	* \brief multiplies the Matrix with a scale value
	* \param rhs a float value
	* \return the result of the multiplication with the scale
	*/
	Matrix2 operator*(float const& rhs) const;

	/**
	* \brief calculates the vector transformation
	* \param rhs a vector object
	* \return the result of the multiplication with the vector and the matrix (transformed vector)
	*/
	Vector2 operator*(Vector2 const& rhs) const;

	friend const Vector2 operator*(const Vector2& lhs, const Matrix2& rhs);

	/**
	* \brief divides the Matrix with a scale value
	* \param rhs a float value
	* \return the result of the division with the scale
	*/
	Matrix2 operator/(float const& rhs) const;

	/**
	 * \brief  calculates the determinant from the matrix
	 * \return float determinant
	 */
	float GetDeterminant() const;

	Matrix2 Triangulate() const;

	Matrix2 GetCoFactor() const;

	Matrix2 Invert() const;

	/**
	* \brief  calculates the transponse from the matrix
	* \return the transponse matrix
	*/
	Matrix2 GetTranspose() const;

	/**
	 * \brief returns an identity matrix
	 * \return A Matrix object
	 */
	static Matrix2 Identity();

};

#endif