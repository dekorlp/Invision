#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "inMath.h"

BOOST_AUTO_TEST_SUITE(MatrixUnitTest);

BOOST_AUTO_TEST_CASE(MatrixInitializationTest)
{
	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	const Matrix res = mat;
	BOOST_CHECK(mat == res);
}

BOOST_AUTO_TEST_CASE(MatrixMultiplicationTest)
{
	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Matrix res = mat * mat2;

	Matrix correctMat(139, 78, 144, 61,
		57, 72, 52, 25,
		81, 69, 98, 40,
		97, 60, 101, 48);

	BOOST_CHECK(res == correctMat);
}


BOOST_AUTO_TEST_CASE(MatrixAdditionTest)
{
	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Matrix res = mat + mat2;

	Matrix correctMat(8, 17, 13, 7,
		12, 5, 9, 5,
		10, 8, 13, 2,
		9, 8, 4, 5);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixSubtractionTest)
{
	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Matrix res = mat - mat2;

	Matrix correctMat(-2, -1, 5, 3,
		2, -1, -7, -3,
		-2, 2, -1, -2,
		-3, 8, 2, 3);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixScaleMultiplicationTest)
{
	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	float scale = 2;

	Matrix res = mat * scale;

	Matrix correctMat(6, 16, 18, 10,
		14, 4, 2, 2,
		8, 10, 12, 0,
		6, 16, 6, 8);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixScaleDivisionTest)
{

	Matrix mat(6, 16, 18, 10,
		14, 4, 2, 2,
		8, 10, 12, 0,
		6, 16, 6, 8);

	Matrix correctMat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	float scale = 2;

	Matrix res = mat / scale;

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixAddAssignTest)
{

	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);

	Matrix correctMat(8, 17, 13, 7,
		12, 5, 9, 5,
		10, 8, 13, 2,
		9, 8, 4, 5);

	mat += mat2;

	BOOST_CHECK(mat == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixSubAssignTest)
{

	Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);

	Matrix correctMat(-2, -1, 5, 3,
		2, -1, -7, -3,
		-2, 2, -1, -2,
		-3, 8, 2, 3);

	mat -= mat2;

	BOOST_CHECK(mat == correctMat);
}

BOOST_AUTO_TEST_CASE(AssignIdentityMatrix)
{
	Matrix identity = Matrix::Identity();

	Matrix correctMat(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	BOOST_CHECK(identity == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixVektorMultiplicationTest)
{
	Matrix mat(5, 5, 1, 5,
		3, 8, 5, 1,
		7, 2, 7, 0,
		3, 8, 3, 4);

	Vector3 vec(4, 3, 2);
	Vector3 correctVec(0.880952358f, 1.09523809f, 1.14285719f);

	Vector3 res = mat * vec;

	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(MatrixVektorRotationTest)
{
	Vector3 position(0.0, 2.0, 6.0);
	Vector3 axis(0.0, 0.0, -1.0);
	Matrix mat = Matrix::RotateAxis(axis, 60.0);
	Vector3 res = mat * position;
	Vector3 correctVector(-1.73205078f, 1.00000000f, 6.00000000f);
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(MatrixVektorRotationTest2)
{
	Vector3 position(0.0, 2.0, 6.0);
	Matrix mat = Matrix::RotateZ(-60.0);
	Vector3 res = mat * position;
	Vector3 correctVector(-1.73205078f, 1.00000000f, 6.00000000f);
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_SUITE_END();