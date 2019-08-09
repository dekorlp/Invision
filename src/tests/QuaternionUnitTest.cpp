#include "stdafx.h"
#include "boost\test\unit_test.hpp"

#include "inMath.h"

BOOST_AUTO_TEST_SUITE(QuaternionUnitTest);

	BOOST_AUTO_TEST_CASE(QuaternionRotationTest)
	{
		// tried to reproduce calculcation on page https://www.uni-koblenz.de/~cg/veranst/ws0001/sem/Bartz.pdf page 6

		Invision::Vector3 position(0.0, 2.0, 6.0);
		Invision::Vector3 axis(0.0, 0.0, -1.0);
		Invision::Quaternion quat(position, 0);
		Invision::Vector3 res = quat.rotate(axis, 60);

		Invision::Vector3 correctVector(-1.73205078f, 1.00000000f, 6.00000000f);
		BOOST_CHECK(res == correctVector);
	}

BOOST_AUTO_TEST_SUITE_END();