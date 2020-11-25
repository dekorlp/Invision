#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "InMath.h"

BOOST_AUTO_TEST_SUITE(MatrixUnitTest);

BOOST_AUTO_TEST_CASE(MatrixInitializationTest)
{
	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	const Invision::Matrix res = mat;
	BOOST_CHECK(mat == res);
}

BOOST_AUTO_TEST_CASE(MatrixMultiplicationTest)
{
	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Invision::Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Invision::Matrix res = mat * mat2;

	Invision::Matrix correctMat(139, 78, 144, 61,
		57, 72, 52, 25,
		81, 69, 98, 40,
		97, 60, 101, 48);

	BOOST_CHECK(res == correctMat);
}


BOOST_AUTO_TEST_CASE(MatrixAdditionTest)
{
	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Invision::Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Invision::Matrix res = mat + mat2;

	Invision::Matrix correctMat(8, 17, 13, 7,
		12, 5, 9, 5,
		10, 8, 13, 2,
		9, 8, 4, 5);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixSubtractionTest)
{
	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Invision::Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);
	Invision::Matrix res = mat - mat2;

	Invision::Matrix correctMat(-2, -1, 5, 3,
		2, -1, -7, -3,
		-2, 2, -1, -2,
		-3, 8, 2, 3);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixScaleMultiplicationTest)
{
	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	float scale = 2;

	Invision::Matrix res = mat * scale;

	Invision::Matrix correctMat(6, 16, 18, 10,
		14, 4, 2, 2,
		8, 10, 12, 0,
		6, 16, 6, 8);

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixScaleDivisionTest)
{

	Invision::Matrix mat(6, 16, 18, 10,
		14, 4, 2, 2,
		8, 10, 12, 0,
		6, 16, 6, 8);

	Invision::Matrix correctMat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	float scale = 2;

	Invision::Matrix res = mat / scale;

	BOOST_CHECK(res == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixAddAssignTest)
{

	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Invision::Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);

	Invision::Matrix correctMat(8, 17, 13, 7,
		12, 5, 9, 5,
		10, 8, 13, 2,
		9, 8, 4, 5);

	mat += mat2;

	BOOST_CHECK(mat == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixSubAssignTest)
{

	Invision::Matrix mat(3, 8, 9, 5,
		7, 2, 1, 1,
		4, 5, 6, 0,
		3, 8, 3, 4);

	Invision::Matrix mat2(5, 9, 4, 2,
		5, 3, 8, 4,
		6, 3, 7, 2,
		6, 0, 1, 1);

	Invision::Matrix correctMat(-2, -1, 5, 3,
		2, -1, -7, -3,
		-2, 2, -1, -2,
		-3, 8, 2, 3);

	mat -= mat2;

	BOOST_CHECK(mat == correctMat);
}

BOOST_AUTO_TEST_CASE(AssignIdentityInvision)
{
	Invision::Matrix identity = Invision::Matrix::Identity();

	Invision::Matrix correctMat(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	BOOST_CHECK(identity == correctMat);
}

BOOST_AUTO_TEST_CASE(MatrixVektorMultiplicationTest)
{
	Invision::Matrix mat(5, 5, 1, 5,
		3, 8, 5, 1,
		7, 2, 7, 0,
		3, 8, 3, 4);

	Invision::Vector3 vec(4, 3, 2);
	Invision::Vector3 correctVec(0.880952358f, 1.09523809f, 1.14285719f);

	Invision::Vector3 res = mat * vec;

	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(MatrixVektorRotationTest)
{
	Invision::Vector3 position(0.0, 2.0, 6.0);
	Invision::Vector3 axis(0.0, 0.0, -1.0);
	Invision::Matrix mat = Invision::Matrix::RotateAxis(axis, 60.0);
	Invision::Vector3 res = mat * position;
	Invision::Vector3 correctVector(-1.73205078f, 1.00000000f, 6.00000000f);
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(MatrixVektorRotationTest2)
{
	Invision::Vector3 position(0.0, 2.0, 6.0);
	Invision::Matrix mat = Invision::Matrix::RotateZ(-60.0);
	Invision::Vector3 res = mat * position;
	Invision::Vector3 correctVector(-1.73205078f, 1.00000000f, 6.00000000f);
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_SUITE_END();