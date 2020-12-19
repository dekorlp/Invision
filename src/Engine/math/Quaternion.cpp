#include "precompiled.h"
#include "Vector3.h"
#include "Matrix.h"

#include "Quaternion.h"
namespace Invision {
	Quaternion::Quaternion()
	{
		w = 0;
		v = Vector3(0, 0, 0);
	}

	Quaternion::Quaternion(Vector3 &v, float a) // rausgenommen zum debuggen
	{
		this->v = Vector3(v.getX(), v.getY(), v.getZ());
		this->w = a;
	}

	Quaternion::Quaternion(Matrix &m)
	{
		float b1_squared = 0.25 * (1.0 + m.At(0, 0) + m.At(1, 1) + m.At(2, 2));
		float b1 = sqrt(b1_squared);
		float over_b1_4 = 0.25 / b1;


		this->w = (float)b1;
		this->v = Vector3(m.At(2, 1) - m.At(1, 2) * over_b1_4, m.At(0, 2) - m.At(2, 0) * over_b1_4, m.At(1, 0) - m.At(0, 1) * over_b1_4);
	}

	const Quaternion Quaternion::invert() const
	{
		Quaternion q;
		q.w = w;

		q.v = Vector3(-v.getX(), -v.getY(), -v.getZ());
		return q;
	}

	const Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		Quaternion r;
		r.w = w * q.w + v.dot(q.v);
		r.v = v * q.w + q.v * w + v.cross(q.v);

		return r;
	}

	const Vector3 Quaternion::operator*(const Vector3& v) const
	{
		Quaternion p;
		p.w = 0;
		p.v = v;

		Vector3 vc = this->v.cross(v);
		return v + vc * (2 * w) + this->v.cross(vc) * 2;
	}

	float Quaternion::length() const
	{
		return sqrtf(powf(v.getX(), 2) + powf(v.getY(), 2) + powf(v.getZ(), 2) + powf(w, 2));
	}

	Quaternion Quaternion::normalize() const
	{
		float nq = length();
		Vector3 vec(v.getX() / nq, v.getY() / nq, v.getZ() / nq);
		return Quaternion(vec, w / nq);
	}

	Vector3 Quaternion::rotate(Vector3 &axis, float angle) const
	{
		axis = Vector3::Normalize(axis);
		angle = angle * PI_F / 180; // convert deg/grad -> rad/bogenmaß

		float c = cosf(angle / 2);
		float s = sinf(angle / 2);

		Vector3 qv(-axis.getX() * s, -axis.getY() * s, -axis.getZ() * s); // left hand coordinate system
		Quaternion qR(qv, c);
		Vector3 res = qR * this->v;
		return res;
	}

	Matrix Quaternion::GetMatrix()
	{
		float qx = v.getX();
		float qy = v.getY();
		float qz = v.getZ();
		float qw = w;

		
		const float n = 1.0f / sqrt(v.getX()*v.getX() + v.getY()*v.getY() + v.getZ()*v.getZ() + w * w);
		qx *= n;
		qy *= n;
		qz *= n;
		qw *= n;

		return Matrix(1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f,
			2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f,
			2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
}