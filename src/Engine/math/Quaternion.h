#ifndef QUATERNION_H
#define QUATERNION_H

#include "base/SIMD.h"



namespace Invision
{
	class Vector3;

	/**
	* \brief Matrix
	*/
	 class Quaternion
	{
	private:
		Vector3 v;
		float w;
	public:
		INVISION_API Quaternion();
		INVISION_API Quaternion(Vector3 &v, float a);
		INVISION_API const Quaternion invert() const;
		INVISION_API const Quaternion operator*(const Quaternion& q) const;
		INVISION_API const Vector3 operator*(const Vector3& v) const;
		INVISION_API float length() const;
		INVISION_API Quaternion normalize() const;
		INVISION_API Vector3 rotate(Vector3& axis, float angle) const;

	};
}
#endif //QUATERNION_H