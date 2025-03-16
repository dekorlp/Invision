#ifndef QUATERNION_H
#define QUATERNION_H
#include "Vector3.h"


class Matrix;
class Quaternion
{
private:
	Vector3 v;
	float w;
public:
	Quaternion();
	Quaternion(Vector3& v, float a);
	Quaternion(Matrix& m);
	const Quaternion invert() const;
	const Quaternion operator*(const Quaternion& q) const;
	const Vector3 operator*(const Vector3& v) const;
	float length() const;
	Quaternion normalize() const;
	Vector3 rotate(Vector3& axis, float angle) const;
	Matrix GetMatrix();

};

#endif