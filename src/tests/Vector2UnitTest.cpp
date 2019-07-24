#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "inMath.h"

BOOST_AUTO_TEST_SUITE(Vektor2UnitTest);
BOOST_AUTO_TEST_CASE(Vector2CorrectAdditionVectorsTest) // +
	{
		Vector2 vec1(1, 4);
		Vector2 vec2(4, 3);
		Vector2 correctVec(5, 7);
		Vector2 res = vec1 + vec2;
		BOOST_CHECK(res == correctVec);
	}

	/*BOOST_AUTO_TEST_CASE(Vector2IncorrectAdditionVectorsTest) // +
	{
		Vector2 vec1(1, 4);
		Vector2 vec2(4, 3);
		Vector2 correctVec(8, 7);
		Vector2 res = vec1 + vec2;
		BOOST_TEST(res != correctVec);
	}*/

	BOOST_AUTO_TEST_CASE(Vector2CalculateCrossProductTest) // *
	{
		Vector2 vec1(2, 8);
		Vector2 vec2(8, 4);
		float correctVal = -56;
		float res = vec1.cross(vec2);
		BOOST_CHECK(res == correctVal);
	}

	BOOST_AUTO_TEST_CASE(Vector2VectorAssignTest) // =
	{
		Vector2 vec1(2, 8);
		Vector2 vec2(4, 8);
		Vector2 vec3(-8, 10);
		Vector2 correctVector(2, 8);
		Vector2 res = vec2;
		res = vec1;
		BOOST_CHECK(res == correctVector);
	}


	BOOST_AUTO_TEST_CASE(Vector2MultiplicateScaleTest) // * scale
	{
		Vector2 vec1(2, 4);
		float scale = 2.0f;
		Vector2 correctVector(4, 8);
		Vector2 res = vec1 * scale;
		BOOST_CHECK(res == correctVector);
	}

	BOOST_AUTO_TEST_CASE(Vector2DivideScaleTest) // / scale
	{
		Vector2 vec1(4, 8);
		float scale = 2.0f;
		Vector2 correctVector(2, 4);
		Vector2 res = vec1 / scale;
		BOOST_CHECK(res == correctVector);
	}

	BOOST_AUTO_TEST_CASE(Vector2DotProductTest) // =
	{
		Vector2 vec1(2, 3);
		Vector2 vec2(3, 5);
		float dotProduct = vec1.dot(vec2);
		float correctValue = 21;
		BOOST_CHECK(dotProduct == correctValue);
	}

	BOOST_AUTO_TEST_CASE(Vector2VectorLengthTest) // length
	{
		Vector2 vec1(2, 4);
		float correctVal(4.47f);
		float res = vec1.length();

		float floatWithTwoCommataCorrectValue = round(correctVal);
		float floatWithTwoCommataRes = round(res);

		BOOST_CHECK(floatWithTwoCommataRes == floatWithTwoCommataCorrectValue);
	}

BOOST_AUTO_TEST_SUITE_END();
