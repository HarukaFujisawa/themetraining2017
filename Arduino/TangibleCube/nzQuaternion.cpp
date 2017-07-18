
#include "nzQuaternion.h"

template<typename TYPE>
_nzMatrix44<TYPE> _nzQuaternion<TYPE>::generateMatrix() const
{
	_nzMatrix44<TYPE> ret;

	// �ｿｽE�ｿｽ�ｿｽn�ｿｽE�ｿｽﾋゑｿｽ�ｿｽ�ｿｽ]
	TYPE s = length();
	s = (TYPE)(2.0f / s * s);
	TYPE vx = i.x * s;
	TYPE vy = i.y * s;
	TYPE vz = i.z * s;
	TYPE wx = vx * r;
	TYPE wy = vy * r;
	TYPE wz = vz * r;
	TYPE sx = i.x * vx;
	TYPE sy = i.y * vy;
	TYPE sz = i.z * vz;
	TYPE cx = i.y * vz;
	TYPE cy = i.z * vx;
	TYPE cz = i.x * vy;
	ret.element(0, 0)    = 1 - sy - sz;
	 ret.element(0, 1)   = cz - wz;
	  ret.element(0, 2)  = cy + wy;
	   ret.element(0, 3) = 0;
	ret.element(1, 0)    = cz + wz;
	 ret.element(1, 1)   = 1 - sx - sz;
	  ret.element(1, 2)  = cx - wx;
	   ret.element(1, 3) = 0;
	ret.element(2, 0)    = cy - wy;
	 ret.element(2, 1)   = cx + wx;
	  ret.element(2, 2)  = 1 - sx - sy;
	   ret.element(2, 3) = 0;
	ret.element(3, 0)    = 0;
	 ret.element(3, 1)   = 0;
	  ret.element(3, 2)  = 0;
	   ret.element(3, 3) = 1;

	return ret;
}

/*
 * �ｿｽE�ｿｽ�ｿｽn�ｿｽE�ｿｽﾋゑｿｽ�ｿｽ�ｿｽ]
 * Q = {w; x, y, z}
 * ArbitraryRotateMatrix = 
 * |1 - 2y^2 - 2z^2     2xy - 2wz        2xz + 2wy      0|
 * |  2xy + 2wz      1 - 2x^2 - 2z^2     2yz - 2wx      0|
 * |  2xz - 2wy         2yz + 2wz     1 - 2x^2 - 2y^2   0|
 * |      0                 0                0          1|
 */
template<typename TYPE>
_nzMatrix44<TYPE> _nzQuaternion<TYPE>::generateArbitraryMatrix() const
{
	_nzMatrix44<TYPE> ret;

	ret.element(0, 0)    = 1 - 2 * (i.y * i.y + i.z * i.z);
	 ret.element(0, 1)   = 2 * (i.x * i.y - r * i.z);
	  ret.element(0, 2)  = 2 * (i.x * i.z + r * i.y);
	   ret.element(0, 3) = 0;
	ret.element(1, 0)    = 2 * (i.x * i.y + r * i.z);
	 ret.element(1, 1)   = 1 - 2 * (i.x * i.x + i.z * i.z);
	  ret.element(1, 2)  = 2 * (i.y * i.z - r * i.x);
	   ret.element(1, 3) = 0;
	ret.element(2, 0)    = 2 * (i.x * i.z - r * i.y);
	 ret.element(2, 1)   = 2 * (i.y * i.z + r * i.z);
	  ret.element(2, 2)  = 1 - 2 * (i.x * i.x + i.y * i.y);
	   ret.element(2, 3) = 0;
	ret.element(3, 0)    = 0;
	 ret.element(3, 1)   = 0;
	  ret.element(3, 2)  = 0;
	   ret.element(3, 3) = 1;
	return ret;
}

template<typename TYPE>
void _nzQuaternion<TYPE>::rotate(TYPE degree, const _nzVector3<TYPE> &axis)
{
	_nzQuaternion<TYPE> q(generateQuaternionAxis(degree, axis));
	rotate(q);
}

template<typename TYPE>
void _nzQuaternion<TYPE>::rotate(const _nzVector3<TYPE> &diff)
{
	_nzVector3<TYPE> d = diff * (float)M_PI / 180.0f;
	_nzQuaternion<TYPE> q(*this);
	r   += (TYPE)(0.5 * (-q.i.x * d.x - q.i.y * d.y - q.i.z * d.z));
	i.x += (TYPE)(0.5 * ( q.r   * d.x - q.i.z * d.y + q.i.y * d.z));
	i.y += (TYPE)(0.5 * ( q.i.z * d.x + q.r   * d.y - q.i.x * d.z));
	i.z += (TYPE)(0.5 * (-q.i.y * d.x + q.i.x * d.y + q.r   * d.z));
	normalize();
}

template<typename TYPE>
void _nzQuaternion<TYPE>::rotate(const _nzQuaternion<TYPE> &q)
{
	_nzQuaternion<TYPE> res = (*this) * q;
	*this = res;
}

template<typename TYPE>
void _nzQuaternion<TYPE>::faceTo(const _nzVector3<TYPE> &vec)
{
	i = _nzVector3<TYPE>(0, 0, 0);
	r = 1;
	_nzVector3<TYPE> v(vec), f(0, 0, 1);
	v.normalize();
	TYPE d = dot(f, v);
	if(1 == d){
	} else if(-1 == d){
		rotateY(180);
	} else {
		_nzVector3<TYPE> normal = cross(f, vec);
		normal.normalize();
		rotate((TYPE)(acos((float)d) * 180.0f / (float)M_PI), normal);
	}
}

template<typename TYPE>
void _nzQuaternion<TYPE>::faceTo(const _nzVector3<TYPE> &vec, const _nzVector3<TYPE> &up)
{
	faceTo(vec);

	// Z�ｿｽ�ｿｽ(0, 0, 1)�ｿｽ�ｿｽ�ｿｽ�ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾄ，Y�ｿｽ�ｿｽ(0, 1, 0)�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ墲ｹ�ｿｽ驍ｽ�ｿｽﾟ，XY�ｿｽ�ｿｽ�ｿｽﾊへの射影�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
	_nzVector3<TYPE> tup(rotateVectorByQuaternion(up, *this)), // �ｿｽ�ｿｽ]�ｿｽ�ｿｽ�ｿｽﾉなゑｿｽ�ｿｽﾄゑｿｽ�ｿｽﾜゑｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
					 tx(rotateVectorByQuaternion(_nzVector3<TYPE>(1, 0, 0), *this)), // �ｿｽ�ｿｽ]�ｿｽ�ｿｽX�ｿｽ�ｿｽ
					 ty(rotateVectorByQuaternion(_nzVector3<TYPE>(0, 1, 0), *this)); // �ｿｽ�ｿｽ]�ｿｽ�ｿｽY�ｿｽ�ｿｽ
	TYPE dx = dot(tx, up), dy = dot(ty, up);	// X�ｿｽ�ｿｽ�ｿｽCY�ｿｽ�ｿｽ�ｿｽﾖの射影
	float angle = -atan2((float)dx, (float)dy);
	rotateZ((TYPE)(angle * 180.0f / (TYPE)M_PI));
}

/*
 * A  = {a; U}
 * B  = {b; V}
 * AB = {ab - U�ｿｽEV; aV + bU + U�ｿｽ~V}
 */
template<typename TYPE>
const _nzQuaternion<TYPE> operator *(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2)
{
	return _nzQuaternion<TYPE>(q1.r * q2.r - dot(q1.i, q2.i), q1.r * q2.i + q2.r * q1.i + cross(q1.i, q2.i));
}

/*
 * Q = {cos(r/2); xsin(r/2), ysin(r/2), zsin(r/2)}
 */
template<typename TYPE>
_nzQuaternion<TYPE> generateQuaternionAxis(TYPE degree, const _nzVector3<TYPE> &axis)
{
	float r = (float)(degree) * (float)M_PI / 180.0f / 2.0f;
	return _nzQuaternion<TYPE>((TYPE)cos(r), (TYPE)sin(r) * axis);
}

template<typename TYPE>
_nzVector3<TYPE> rotateVectorByQuaternion(const _nzVector3<TYPE> &v, const _nzQuaternion<TYPE> &q)
{
	_nzQuaternion<TYPE> p(0, v);
	_nzQuaternion<TYPE> res = q * p * q.conjugate();	// �ｿｽE�ｿｽ�ｿｽn�ｿｽE�ｿｽﾋゑｿｽ�ｿｽ�ｿｽ]
	return res.i;
}


// �ｿｽe�ｿｽ�ｿｽ�ｿｽv�ｿｽ�ｿｽ�ｿｽ[�ｿｽg�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽp�ｿｽ}�ｿｽN�ｿｽ�ｿｽ.
#define REALIZE_TEMPLATE_NZ_QUATERNION(TYPE) \
	template _nzMatrix44<TYPE> _nzQuaternion<TYPE>::generateMatrix() const; \
	template _nzMatrix44<TYPE> _nzQuaternion<TYPE>::generateArbitraryMatrix() const; \
	template void _nzQuaternion<TYPE>::rotate(const _nzQuaternion<TYPE> &q); \
	template void _nzQuaternion<TYPE>::rotate(TYPE degree, const _nzVector3<TYPE> &axis); \
	template void _nzQuaternion<TYPE>::rotate(const _nzVector3<TYPE> &diff); \
	template void _nzQuaternion<TYPE>::faceTo(const _nzVector3<TYPE> &vec); \
	template void _nzQuaternion<TYPE>::faceTo(const _nzVector3<TYPE> &vec, const _nzVector3<TYPE> &up); \
	template const _nzQuaternion<TYPE> operator *(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2); \
	template _nzQuaternion<TYPE> generateQuaternionAxis(TYPE degree, const _nzVector3<TYPE> &axis); \
	template _nzVector3<TYPE> rotateVectorByQuaternion(const _nzVector3<TYPE> &v, const _nzQuaternion<TYPE> &q);\

REALIZE_TEMPLATE_NZ_QUATERNION(double)
REALIZE_TEMPLATE_NZ_QUATERNION(float)
REALIZE_TEMPLATE_NZ_QUATERNION(int)


