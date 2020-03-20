#ifndef QUATERNION_H
#define QUATERNION_H

#include "math_config.h"
#include "Simd_Config.h"
#include "sse_mathfun.h"
#include "Vector3.h"


namespace Invision
{
	/**
	* \brief Matrix
	*/
	class INVISION_API Quaternion
	{
	private:
		Vector3 v;
		float w;
	public:
		Quaternion();
		Quaternion(Vector3 &v, float a);
		const Quaternion invert() const;
		const Quaternion operator*(const Quaternion& q) const;
		const Vector3 operator*(const Vector3& v) const;
		float length() const;
		Quaternion normalize() const;
		Vector3 rotate(Vector3& axis, float angle) const;

	};
}
#endif //QUATERNION_H