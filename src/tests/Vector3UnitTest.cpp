#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "inMath.h"

BOOST_AUTO_TEST_SUITE(Vektor3UnitTest);
BOOST_AUTO_TEST_CASE(Vector3CorrectAdditionVectorsTest) // +
{
	Invision::Vector3 vec1(1, 4, 2);
	Invision::Vector3 vec2(4, 3, 1);
	Invision::Vector3 correctVec(5, 7, 3);
	Invision::Vector3 res = vec1 + vec2;
	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(Vector3CalculateCrossProductTest) // *
{
	Invision::Vector3 vec1(2, 8, 4);
	Invision::Vector3 vec2(4, 8, 3);
	Invision::Vector3 correctVec(-8, 10, -16);
	Invision::Vector3 res = vec1.cross(vec2);
	BOOST_CHECK(res == correctVec);
}

BOOST_AUTO_TEST_CASE(Vector3VectorAssignTest) // =
{
	Invision::Vector3 vec1(2, 8, 4);
	Invision::Vector3 vec2(4, 8, 3);
	Invision::Vector3 vec3(-8, 10, -16);
	Invision::Vector3 correctVector(2, 8, 4);
	Invision::Vector3 res = vec2;
	res = vec1;
	BOOST_CHECK(res == correctVector);
}


BOOST_AUTO_TEST_CASE(Vector3MultiplicateScaleTest) // * scale
{
	Invision::Vector3 vec1(2, 4, 8);
	float scale = 2.0f;
	Invision::Vector3 correctVector(4, 8, 16);
	Invision::Vector3 res = vec1 * scale;
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(Vector3DivideScaleTest) // / scale
{
	Invision::Vector3 vec1(4, 8, 16);
	float scale = 2.0f;
	Invision::Vector3 correctVector(2, 4, 8);
	Invision::Vector3 res = vec1 / scale;
	BOOST_CHECK(res == correctVector);
}

BOOST_AUTO_TEST_CASE(Vector3DotProductTest) // =
{
	Invision::Vector3 vec1(2, 4, 8);
	Invision::Vector3 vec2(3, 5, 6);
	float dotProduct = vec1.dot(vec2);
	float correctValue = 74;
	BOOST_CHECK(dotProduct == correctValue);
}

BOOST_AUTO_TEST_CASE(Vector3VectorLengthTest) // length
{
	Invision::Vector3 vec1(2, 4, 8);
	float correctVal(9.16f);
	float res = vec1.length();

	float floatWithTwoCommataCorrectValue = (float)((int)(correctVal * 100)) / 100;
	float floatWithTwoCommataRes = (float)((int)(res * 100)) / 100;

	BOOST_CHECK(floatWithTwoCommataRes == floatWithTwoCommataCorrectValue);
}

BOOST_AUTO_TEST_SUITE_END();