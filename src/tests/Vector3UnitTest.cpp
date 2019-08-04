#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "inMath.h"

BOOST_AUTO_TEST_SUITE(Vektor3UnitTest);
BOOST_AUTO_TEST_CASE(Vector3CorrectAdditionVectorsTest) // +
{
	Vector3 vec1(1, 4, 2);
	Vector3 vec2(4, 3, 1);
	Vector3 correctVec(5, 7, 3);
	Vector3 res = vec1 + vec2;
	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(Vector3CalculateCrossProductTest) // *
{
	Vector3 vec1(2, 8, 4);
	Vector3 vec2(4, 8, 3);
	Vector3 correctVec(-8, 10, -16);
	Vector3 res = vec1.cross(vec2);
	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(Vector3VectorAssignTest) // =
{
	Vector3 vec1(2, 8, 4);
	Vector3 vec2(4, 8, 3);
	Vector3 vec3(-8, 10, -16);
	Vector3 correctVector(2, 8, 4);
	Vector3 res = vec2;
	res = vec1;
	BOOST_CHECK(res == correctVector);
}


BOOST_AUTO_TEST_CASE(Vector3MultiplicateScaleTest) // * scale
{
	Vector3 vec1(2, 4, 8);
	float scale = 2.0f;
	Vector3 correctVector(4, 8, 16);
	Vector3 res = vec1 * scale;
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(Vector3DivideScaleTest) // / scale
{
	Vector3 vec1(4, 8, 16);
	float scale = 2.0f;
	Vector3 correctVector(2, 4, 8);
	Vector3 res = vec1 / scale;
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(Vector3DotProductTest) // =
{
	Vector3 vec1(2, 4, 8);
	Vector3 vec2(3, 5, 6);
	float dotProduct = vec1.dot(vec2);
	float correctValue = 74;
	BOOST_CHECK(dotProduct == correctValue);
}

BOOST_AUTO_TEST_CASE(Vector3VectorLengthTest) // length
{
	Vector3 vec1(2, 4, 8);
	float correctVal(9.16f);
	float res = vec1.length();

	float floatWithTwoCommataCorrectValue = (float)((int)(correctVal * 100)) / 100;
	float floatWithTwoCommataRes = (float)((int)(res * 100)) / 100;

	BOOST_CHECK(floatWithTwoCommataRes == floatWithTwoCommataCorrectValue);
}

BOOST_AUTO_TEST_SUITE_END();