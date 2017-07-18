

#ifndef NZ_VECTOR_H
#define NZ_VECTOR_H

#include <math.h>

/**
 *	@addtogroup Graphics_eXperimental
 *	@{
 *	@file		nzVector.h
 *	@author		NARITA Tomoya
 *
 *	@warning
 *	Copyright(C) 2010 Sony Corporation<br>
 *	All Rights Reserved.<br>
 *
 */

template<typename TYPE> struct _nzVector2{
	TYPE x;
	TYPE y;
	_nzVector2() : x(0), y(0){}
	_nzVector2(TYPE ix, TYPE iy) : x(ix), y(iy) {}
	TYPE length() const {return (TYPE)sqrt((float)((x * x) + (y * y)));}
	void normalize() {TYPE l = length(); if(l != 0){x /= l; y /= l;}}
	static const _nzVector2<TYPE> xaxis() {return _nzVector2<TYPE>(1, 0);}
	static const _nzVector2<TYPE> yaxis() {return _nzVector2<TYPE>(0, 1);}
};

template<typename TYPE> struct _nzVector3{
	TYPE x;
	TYPE y;
	TYPE z;
	_nzVector3() : x(0), y(0), z(0){}
	_nzVector3(TYPE ix, TYPE iy, TYPE iz) : x(ix), y(iy), z(iz) {}
	TYPE length() const {return (TYPE)sqrt((float)((x * x) + (y * y) + (z * z)));}
	void normalize() {TYPE l = length(); if(l != 0){x /= l; y /= l; z /= l;}}
	static const _nzVector3<TYPE> xaxis() {return _nzVector3<TYPE>(1, 0, 0);}
	static const _nzVector3<TYPE> yaxis() {return _nzVector3<TYPE>(0, 1, 0);}
	static const _nzVector3<TYPE> zaxis() {return _nzVector3<TYPE>(0, 0, 1);}
};

template<typename TYPE> struct _nzVector4{
	TYPE x;
	TYPE y;
	TYPE z;
	TYPE w;
	_nzVector4() : x(0), y(0), z(0), w(0){}
	_nzVector4(TYPE ix, TYPE iy, TYPE iz, TYPE iw) : x(ix), y(iy), z(iz), w(iw) {}
	TYPE length() const {return (TYPE)sqrt((float)((x * x) + (y * y) + (z * z) + (w * w)));}
	void normalize() {TYPE l = length(); if(l != 0){x /= l; y /= l; z /= l; w /= l;}}
	static const _nzVector4<TYPE> xaxis() {return _nzVector4<TYPE>(1, 0, 0, 0);}
	static const _nzVector4<TYPE> yaxis() {return _nzVector4<TYPE>(0, 1, 0, 0);}
	static const _nzVector4<TYPE> zaxis() {return _nzVector4<TYPE>(0, 0, 1, 0);}
	static const _nzVector4<TYPE> waxis() {return _nzVector4<TYPE>(0, 0, 0, 1);}
};

typedef struct _nzVector2<double> nzVector2d;
typedef struct _nzVector3<double> nzVector3d;
typedef struct _nzVector4<double> nzVector4d;

typedef struct _nzVector2<float> nzVector2f;
typedef struct _nzVector3<float> nzVector3f;
typedef struct _nzVector4<float> nzVector4f;

typedef struct _nzVector2<int> nzVector2i;
typedef struct _nzVector3<int> nzVector3i;
typedef struct _nzVector4<int> nzVector4i;

typedef struct _nzVector2<short> nzVector2s;
typedef struct _nzVector3<short> nzVector3s;
typedef struct _nzVector4<short> nzVector4s;

typedef struct _nzVector2<unsigned short> nzVector2us;
typedef struct _nzVector3<unsigned short> nzVector3us;
typedef struct _nzVector4<unsigned short> nzVector4us;

typedef struct _nzVector2<unsigned char> nzVector2b;
typedef struct _nzVector3<unsigned char> nzVector3b;
typedef struct _nzVector4<unsigned char> nzVector4b;

typedef struct _nzVector2<unsigned char> nzVector2ub;
typedef struct _nzVector3<unsigned char> nzVector3ub;
typedef struct _nzVector4<unsigned char> nzVector4ub;


/**
 *
 */
template<typename TYPE>                   const inline _nzVector2<TYPE> operator +(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return _nzVector2<TYPE>(vec1.x + vec2.x, vec1.y + vec2.y);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector2<TYPE> operator +(const _nzVector2<TYPE> &vec, SUBTYPE val) {return _nzVector2<TYPE>(vec.x + val, vec.y + val);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector2<TYPE> operator +(SUBTYPE val, const _nzVector2<TYPE> &vec) {return vec + val;}
template<typename TYPE>                         inline _nzVector2<TYPE> operator +=(_nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1 = vec1 + vec2;}

template<typename TYPE>                   const inline _nzVector3<TYPE> operator +(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return _nzVector3<TYPE>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector3<TYPE> operator +(const _nzVector3<TYPE> &vec, SUBTYPE val) {return _nzVector3<TYPE>(vec.x + val, vec.y + val, vec.z + val);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector3<TYPE> operator +(SUBTYPE val, const _nzVector3<TYPE> &vec) {return vec + val;}
template<typename TYPE>                         inline _nzVector3<TYPE> operator +=(_nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1 = vec1 + vec2;}

template<typename TYPE> const inline _nzVector4<TYPE> operator +(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return _nzVector4<TYPE>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);}
template<typename TYPE> const inline _nzVector4<TYPE> operator +(const _nzVector4<TYPE> &vec, TYPE val) {return _nzVector4<TYPE>(vec.x + val, vec.y + val, vec.z + val, vec.w + val);}
template<typename TYPE> const inline _nzVector4<TYPE> operator +(TYPE val, const _nzVector4<TYPE> &vec) {return vec + val;}
template<typename TYPE>       inline _nzVector4<TYPE> operator +=(_nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1 = vec1 + vec2;}

/**
 *
 */
template<typename TYPE>                   const inline _nzVector2<TYPE> operator *(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return _nzVector2<TYPE>(vec1.x * vec2.x, vec1.y * vec2.y);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector2<TYPE> operator *(const _nzVector2<TYPE> &vec, SUBTYPE val) {return _nzVector2<TYPE>(vec.x * val, vec.y * val);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector2<TYPE> operator *(SUBTYPE val, const _nzVector2<TYPE> &vec) {return vec * val;}
template<typename TYPE>                         inline _nzVector2<TYPE> operator *=(_nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1 = vec1 * vec2;}

template<typename TYPE>                   const inline _nzVector3<TYPE> operator *(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return _nzVector3<TYPE>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector3<TYPE> operator *(const _nzVector3<TYPE> &vec, SUBTYPE val) {return _nzVector3<TYPE>(vec.x * val, vec.y * val, vec.z * val);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector3<TYPE> operator *(SUBTYPE val, const _nzVector3<TYPE> &vec) {return vec * val;}
template<typename TYPE>                         inline _nzVector3<TYPE> operator *=(_nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1 = vec1 * vec2;}

template<typename TYPE>                   const inline _nzVector4<TYPE> operator *(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return _nzVector4<TYPE>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector4<TYPE> operator *(const _nzVector4<TYPE> &vec, SUBTYPE val) {return _nzVector4<TYPE>(vec.x * val, vec.y * val, vec.z * val, vec.w * val);}
template<typename TYPE, typename SUBTYPE> const inline _nzVector4<TYPE> operator *(SUBTYPE val, const _nzVector4<TYPE> &vec) {return vec * val;}
template<typename TYPE>                         inline _nzVector4<TYPE> operator *=(_nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1 = vec1 * vec2;}


/**
 *
 */
template<typename TYPE> inline _nzVector2<TYPE> operator -(const _nzVector2<TYPE> &vec) {return vec * -1;}
template<typename TYPE> inline _nzVector2<TYPE> operator -(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return _nzVector2<TYPE>(vec1.x - vec2.x, vec1.y - vec2.y);}
template<typename TYPE, typename SUBTYPE> inline _nzVector2<TYPE> operator -(const _nzVector2<TYPE> &vec, SUBTYPE val) {return _nzVector2<TYPE>(vec.x - val, vec.y - val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector2<TYPE> operator -(SUBTYPE val, const _nzVector2<TYPE> &vec) {return -vec + val;}
template<typename TYPE> inline _nzVector2<TYPE> operator -=(_nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1 = vec1 - vec2;}

template<typename TYPE> inline _nzVector3<TYPE> operator -(const _nzVector3<TYPE> &vec) {return vec * -1;}
template<typename TYPE> inline _nzVector3<TYPE> operator -(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return _nzVector3<TYPE>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);}
template<typename TYPE, typename SUBTYPE> inline _nzVector3<TYPE> operator -(const _nzVector3<TYPE> &vec, SUBTYPE val) {return _nzVector3<TYPE>(vec.x - val, vec.y - val, vec.z - val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector3<TYPE> operator -(SUBTYPE val, const _nzVector3<TYPE> &vec) {return -vec + val;}
template<typename TYPE> inline _nzVector3<TYPE> operator -=(_nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1 = vec1 - vec2;}

template<typename TYPE> inline _nzVector4<TYPE> operator -(const _nzVector4<TYPE> &vec) {return vec * -1;}
template<typename TYPE> inline _nzVector4<TYPE> operator -(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return _nzVector4<TYPE>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w);}
template<typename TYPE, typename SUBTYPE> inline _nzVector4<TYPE> operator -(const _nzVector4<TYPE> &vec, SUBTYPE val) {return _nzVector4<TYPE>(vec.x - val, vec.y - val, vec.z - val, vec.w - val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector4<TYPE> operator -(SUBTYPE val, const _nzVector4<TYPE> &vec) {return -vec + val;}
template<typename TYPE> inline _nzVector4<TYPE> operator -=(_nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1 = vec1 - vec2;}

/**
 *
 */
template<typename TYPE> inline _nzVector2<TYPE> operator /(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return _nzVector2<TYPE>(vec1.x / vec2.x, vec1.y / vec2.y);}
template<typename TYPE, typename SUBTYPE> inline _nzVector2<TYPE> operator /(const _nzVector2<TYPE> &vec, SUBTYPE val) {return _nzVector2<TYPE>(vec.x / val, vec.y / val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector2<TYPE> operator /(SUBTYPE val, const _nzVector2<TYPE> &vec) {return _nzVector2<TYPE>(val / vec.x, val / vec.y);}
template<typename TYPE> inline _nzVector2<TYPE> operator /=(_nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1 = vec1 / vec2;}

template<typename TYPE> inline _nzVector3<TYPE> operator /(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return _nzVector3<TYPE>(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z);}
template<typename TYPE, typename SUBTYPE> inline _nzVector3<TYPE> operator /(const _nzVector3<TYPE> &vec, SUBTYPE val) {return _nzVector3<TYPE>(vec.x / val, vec.y / val, vec.z / val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector3<TYPE> operator /(SUBTYPE val, const _nzVector3<TYPE> &vec) {return _nzVector3<TYPE>(val / vec.x, val / vec.y, val / vec.z);}
template<typename TYPE> inline _nzVector3<TYPE> operator /=(_nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1 = vec1 / vec2;}

template<typename TYPE> inline _nzVector4<TYPE> operator /(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return _nzVector4<TYPE>(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z, vec1.w / vec2.w);}
template<typename TYPE, typename SUBTYPE> inline _nzVector4<TYPE> operator /(const _nzVector4<TYPE> &vec, SUBTYPE val) {return _nzVector4<TYPE>(vec.x / val, vec.y / val, vec.z / val, vec.w / val);}
template<typename TYPE, typename SUBTYPE> inline _nzVector4<TYPE> operator /(SUBTYPE val, const _nzVector4<TYPE> &vec) {return _nzVector4<TYPE>(val / vec.x, val / vec.y, val / vec.z, val / vec.w);}
template<typename TYPE> inline _nzVector4<TYPE> operator /=(_nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1 = vec1 / vec2;}

/**
 *
 */
template<typename TYPE> inline bool operator ==(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1.x == vec2.x && vec1.y == vec2.y;}
template<typename TYPE> inline bool operator ==(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;}
template<typename TYPE> inline bool operator ==(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z && vec1.w == vec2.w;}
template<typename TYPE> inline bool operator !=(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return !(vec1 == vec2);}
template<typename TYPE> inline bool operator !=(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return !(vec1 == vec2);}
template<typename TYPE> inline bool operator !=(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return !(vec1 == vec2);}

/**
 *	�ｿｽ�ｿｽ�ｿｽﾏゑｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> inline TYPE dot(const _nzVector2<TYPE> &vec1, const _nzVector2<TYPE> &vec2) {return vec1.x * vec2.x + vec1.y * vec2.y;}
/**
 *	�ｿｽ�ｿｽ�ｿｽﾏゑｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> inline TYPE dot(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;}
/**
 *	�ｿｽ�ｿｽ�ｿｽﾏゑｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> inline TYPE dot(const _nzVector4<TYPE> &vec1, const _nzVector4<TYPE> &vec2) {return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;}

/**
 *	�ｿｽO�ｿｽﾏゑｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> inline _nzVector3<TYPE> cross(const _nzVector3<TYPE> &vec1, const _nzVector3<TYPE> &vec2) {return _nzVector3<TYPE>(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);}

/**
 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆ面の鯉ｿｽ_�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 *	@param ptLine1, ptLine2	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾊゑｿｽ2�ｿｽ_
 *	@param ptPlane1, ptPlane2, ptPlane3 �ｿｽﾊゑｿｽ�ｿｽﾊゑｿｽ3�ｿｽ_
 */
template<typename TYPE>
bool nzIntersection(const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2, const _nzVector3<TYPE> &ptPlane1, const _nzVector3<TYPE> &ptPlane2, const _nzVector3<TYPE> &ptPlane3, _nzVector3<TYPE> &ptIntersection);

/**
 *	�ｿｽ沍�ｿｽ�ｿｽﾌ抵ｿｽ�ｿｽ�ｿｽ�ｿｽﾌ鯉ｿｽ_�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 *	@param pt1Line1, pt2Line1 �ｿｽ�ｿｽ�ｿｽ�ｿｽ1�ｿｽ�ｿｽ�ｿｽﾊゑｿｽ2�ｿｽ_
 *  @param pt1Line2, pt2Line2 �ｿｽ�ｿｽ�ｿｽ�ｿｽ2�ｿｽ�ｿｽ�ｿｽﾊゑｿｽ2�ｿｽ_
 */
template<typename TYPE>
bool nzIntersection(const _nzVector2<TYPE> &pt1Line1, const _nzVector2<TYPE> &pt2Line1, const _nzVector2<TYPE> &pt1Line2, const _nzVector2<TYPE> &pt2Line2, _nzVector2<TYPE> &ptIntersection);

/**
 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆ点�ｿｽﾌ具ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 *	@param ptLine1, ptLine2	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌ端�ｿｽ_
 *	@param point  �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌ具ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ�ｿｽ�ｿｽ�ｿｽ_
 */
template <typename TYPE>
TYPE nzDistance(const _nzVector3<TYPE> &point, const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2);

/**
 *	�ｿｽQ�ｿｽ_�ｿｽﾌ具ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> TYPE inline nzDistance(_nzVector2<TYPE> &p1, _nzVector2<TYPE> &p2) {return (p2 - p1).length();}
/**
 *	�ｿｽQ�ｿｽ_�ｿｽﾌ具ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> TYPE inline nzDistance(_nzVector3<TYPE> &p1, _nzVector3<TYPE> &p2) {return (p2 - p1).length();}
/**
 *	�ｿｽQ�ｿｽ_�ｿｽﾌ具ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟゑｿｽ
 */
template<typename TYPE> TYPE inline nzDistance(_nzVector4<TYPE> &p1, _nzVector4<TYPE> &p2) {return (p2 - p1).length();}

/**
 *
 */

/**	@} */
#endif // NZ_VECTOR_H


