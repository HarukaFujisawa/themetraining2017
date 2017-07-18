
#ifndef NZ_QUATERNION_H
#define NZ_QUATERNION_H

#define _USE_MATH_DEFINES	// math.h�ｿｽﾌ抵ｿｽ`�ｿｽ�ｿｽ�ｿｽg�ｿｽp�ｿｽ�ｿｽ�ｿｽ�ｿｽ

#include "nzVector.h"
#include "nzMatrix.h"
#include <math.h>

/**
 *	@addtogroup Graphics_eXperimental
 *	@{
 *	@file		nzQuaternion.h
 *	@author		NARITA Tomoya
 *
 *	@warning
 *	Copyright(C) 2010 Sony Corporation<br>
 *	All Rights Reserved.<br>
 *
 *	�ｿｽQ�ｿｽlURL:
 *	�ｿｽl�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ http://www015.upp.so-net.ne.jp/notgeld/quaternion.html
 *	�ｿｽ�ｿｽ�ｿｽ~�ｿｽﾂゑｿｽ�ｿｽ�ｿｽ[�ｿｽﾇゑｿｽ�ｿｽﾆコ�ｿｽ�ｿｽ http://marupeke296.com/DXG_No58_RotQuaternionTrans.html
 *	
 */

template<typename TYPE> struct _nzMatrix44;

//struct _nzQuaternion;
//struct _nzQuaternionf operator /(const struct _nzQuaternionf &q, float f);

template<typename TYPE> struct _nzQuaternion{
	TYPE r;		// Real Part
	_nzVector3<TYPE> i;	// Imaginary Part

	/**
	 *
	 */
	_nzQuaternion(TYPE w, TYPE x, TYPE y, TYPE z) : r(w), i(x, y, z) {}
	_nzQuaternion(TYPE w, const _nzVector3<TYPE> &v) : r(w), i(v) {}
	_nzQuaternion(TYPE x, TYPE y, TYPE z) : r(1), i(x, y, z) {}
	_nzQuaternion(const _nzVector3<TYPE> &v) : r(1), i(v) {}
	_nzQuaternion() : r(1) {}

	/**
	 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ
	 */
	struct _nzQuaternion<TYPE> conjugate() const {return _nzQuaternion(r, -i);}

	/**
	 *
	 */
	struct _nzQuaternion<TYPE> invert() const {float norm = length(); return conjugate() / (norm * norm);}

	/**
	 *	�ｿｽ蛯ｫ�ｿｽ�ｿｽ�ｿｽi�ｿｽm�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽj
	 */
	TYPE length() const {return (TYPE)sqrt((float)(r * r + i.x * i.x + i.y * i.y + i.z * i.z));}
	TYPE norm() const {return length();}

	/**
	 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ]�ｿｽs�ｿｽ�ｿｽﾏ奇ｿｽ
	 */
	_nzMatrix44<TYPE> generateMatrix() const;

	/**
	 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽC�ｿｽﾓ行�ｿｽ�ｿｽﾏ奇ｿｽ
	 */
	_nzMatrix44<TYPE> generateArbitraryMatrix() const;

	/**
	 *	�ｿｽw�ｿｽ閧ｵ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void rotate(TYPE degree, const _nzVector3<TYPE> &axis);

	/**
	 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽl�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽB�ｿｽW�ｿｽ�ｿｽ�ｿｽC�ｿｽ�ｿｽ�ｿｽﾌ難ｿｽ�ｿｽﾍ難ｿｽ�ｿｽ�ｿｽ�ｿｽg�ｿｽ�ｿｽ�ｿｽﾆゑｿｽ
	 */
	void rotate(const _nzVector3<TYPE> &diffDegree);

	/**
	 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void rotate(const _nzQuaternion<TYPE> &q);

	/**
	 *	X�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void rotateX(TYPE degree) {rotate(degree, _nzVector3<TYPE>(1, 0, 0));}

	/**
	 *	Y�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void rotateY(TYPE degree) {rotate(degree, _nzVector3<TYPE>(0, 1, 0));}

	/**
	 *	Z�ｿｽ�ｿｽ�ｿｽﾅ会ｿｽ]�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void rotateZ(TYPE degree) {rotate(degree, _nzVector3<TYPE>(0, 0, 1));}

	/**
	 *	�ｿｽw�ｿｽ閧ｵ�ｿｽ�ｿｽ�ｿｽx�ｿｽN�ｿｽg�ｿｽ�ｿｽ�ｿｽﾌ包ｿｽ�ｿｽ�ｿｽ�ｿｽﾉ鯉ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void faceTo(const _nzVector3<TYPE> &vec);

	/**
	 *	�ｿｽw�ｿｽ閧ｵ�ｿｽ�ｿｽ�ｿｽx�ｿｽN�ｿｽg�ｿｽ�ｿｽ�ｿｽﾌ包ｿｽ�ｿｽ�ｿｽ�ｿｽﾉ鯉ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void faceTo(const _nzVector3<TYPE> &vec, const _nzVector3<TYPE> &up);

	/**
	 *	�ｿｽ�ｿｽ�ｿｽK�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	void normalize() {TYPE l = length(); r = r / l; i = i / l;}

	/**
	 *	�ｿｽP�ｿｽﾊク�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ
	 */
	static _nzQuaternion<TYPE> unit() {return _nzQuaternion<TYPE>(1, 0, 0, 0);}

	/**
	 *	�ｿｽP�ｿｽﾊク�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ
	 */
	static _nzQuaternion<TYPE> identity() {return unit();}
};

typedef struct _nzQuaternion<double> nzQuaterniond;
typedef struct _nzQuaternion<float>  nzQuaternionf;
typedef struct _nzQuaternion<int>    nzQuaternioni;


/**
 *	�ｿｽ�ｿｽZ
 */
template<typename TYPE> const        _nzQuaternion<TYPE> operator *(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2);
template<typename TYPE> const inline _nzQuaternion<TYPE> operator *(const _nzQuaternion<TYPE> &q, float f) {return _nzQuaternion<TYPE>(q.r * f, q.i * f);}
template<typename TYPE> const inline _nzQuaternion<TYPE> operator *(float f, const _nzQuaternion<TYPE> &q) {return q * f;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const inline _nzQuaternion<TYPE> operator /(const _nzQuaternion<TYPE> &q, float f) {return _nzQuaternion<TYPE>(q.r / f, q.i / f);}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const inline _nzQuaternion<TYPE> operator +(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2) {return _nzQuaternion<TYPE>(q1.r + q2.r, q1.i + q2.i);}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const inline _nzQuaternion<TYPE> operator -(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2) {return _nzQuaternion<TYPE>(q1.r - q2.r, (const _nzVector3<TYPE>&)(q1.i - q2.i));}

/**
 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ
 */
template<typename TYPE> inline bool operator ==(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2) {return q1.r == q2.r && q1.i == q2.i;}
template<typename TYPE> inline bool operator !=(const _nzQuaternion<TYPE> &q1, const _nzQuaternion<TYPE> &q2) {return !(q1 == q2);}

/**
 *	�ｿｽC�ｿｽﾓ趣ｿｽ�ｿｽﾌ趣ｿｽ�ｿｽ�ｿｽﾌ会ｿｽ]
 *  axis�ｿｽﾍ撰ｿｽ�ｿｽK�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾄゑｿｽ�ｿｽ驍ｱ�ｿｽ�ｿｽ
 */
template<typename TYPE> _nzQuaternion<TYPE> generateQuaternionAxis(TYPE degree, const _nzVector3<TYPE> &axis);

/**
 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽﾅベ�ｿｽN�ｿｽg�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ]	
 */
template<typename TYPE> _nzVector3<TYPE> rotateVectorByQuaternion(const _nzVector3<TYPE> &v, const _nzQuaternion<TYPE> &q);

/**	@} */
#endif // NZ_QUATERNION_H

