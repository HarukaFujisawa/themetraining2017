
#include "nzVector.h"
#include "nzMatrix.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define NZ_MATRIX_SIGN(A) (((A) >= 0) ? 1 : -1)

/*******************************************
 *	nzMatrix44
 *******************************************/

template<typename TYPE>
_nzMatrix44<TYPE> _nzMatrix44<TYPE>::unit()
{
	_nzMatrix44<TYPE> ret;
	memset(&ret, 0, sizeof(_nzMatrix44<TYPE>));
	ret.m_element[0][0] = 1;
	ret.m_element[1][1] = 1;
	ret.m_element[2][2] = 1;
	ret.m_element[3][3] = 1;
	return ret;
}

template<typename TYPE>
void _nzMatrix44<TYPE>::translate(TYPE x, TYPE y, TYPE z)
{
	_nzMatrix44<TYPE> mat(_nzMatrix44<TYPE>::unit());
	mat.m_element[3][0] = x;
	mat.m_element[3][1] = y;
	mat.m_element[3][2] = z;
	(*this) = mat * (*this);
}

template<typename TYPE>
void _nzMatrix44<TYPE>::scale(TYPE x, TYPE y, TYPE z)
{
	_nzMatrix44<TYPE> mat(_nzMatrix44<TYPE>::unit());
	mat.m_element[0][0] = x;
	mat.m_element[1][1] = y;
	mat.m_element[2][2] = z;
	(*this) = mat * (*this);
}

template<typename TYPE>
void _nzMatrix44<TYPE>::rotate(const _nzQuaternion<TYPE> &q)
{
	_nzMatrix44<TYPE> mat(q.generateMatrix());
	(*this) = mat * (*this);
}

template<typename TYPE>
_nzMatrix44<TYPE> _nzMatrix44<TYPE>::invert() const
{
	_nzMatrix44<TYPE> ret;
	TYPE s[4][4];
	memcpy(s, m_element, 4 * 4 * sizeof(TYPE));

	TYPE det = determinant();
	if(0 != det){
		ret.m_element[0][0] = (s[1][1] * s[2][2] * s[3][3] + s[2][1] * s[3][2] * s[1][3] + s[3][1] * s[1][2] * s[2][3] - s[1][1] * s[3][2] * s[2][3] - s[2][1] * s[1][2] * s[3][3] - s[3][1] * s[2][2] * s[1][3]) / det;
		ret.m_element[1][0] = (s[1][0] * s[2][3] * s[3][2] + s[1][2] * s[2][0] * s[3][3] + s[1][3] * s[2][2] * s[3][0] - s[1][0] * s[2][2] * s[3][3] - s[1][2] * s[2][3] * s[3][0] - s[1][3] * s[2][0] * s[3][2]) / det;
		ret.m_element[2][0] = (s[1][0] * s[2][1] * s[3][3] + s[1][1] * s[2][3] * s[3][0] + s[1][3] * s[2][0] * s[3][1] - s[1][0] * s[2][3] * s[3][1] - s[1][1] * s[2][0] * s[3][3] - s[1][3] * s[2][1] * s[3][0]) / det;
		ret.m_element[3][0] = (s[1][0] * s[2][2] * s[3][1] + s[1][1] * s[2][0] * s[3][2] + s[1][2] * s[2][1] * s[3][0] - s[1][0] * s[2][1] * s[3][2] - s[1][1] * s[2][2] * s[3][0] - s[1][2] * s[2][0] * s[3][1]) / det;
		ret.m_element[0][1] = (s[0][1] * s[3][2] * s[2][3] + s[2][1] * s[0][2] * s[3][3] + s[3][1] * s[2][2] * s[0][3] - s[0][1] * s[2][2] * s[3][3] - s[2][1] * s[3][2] * s[0][3] - s[3][1] * s[0][2] * s[2][3]) / det;
		ret.m_element[1][1] = (s[0][0] * s[2][2] * s[3][3] + s[2][0] * s[3][2] * s[0][3] + s[3][0] * s[0][2] * s[2][3] - s[0][0] * s[3][2] * s[2][3] - s[2][0] * s[0][2] * s[3][3] - s[3][0] * s[2][2] * s[0][3]) / det;
		ret.m_element[2][1] = (s[0][0] * s[2][3] * s[3][1] + s[0][1] * s[2][0] * s[3][3] + s[0][3] * s[2][1] * s[3][0] - s[0][0] * s[2][1] * s[3][3] - s[0][1] * s[2][3] * s[3][0] - s[0][3] * s[2][0] * s[3][1]) / det;
		ret.m_element[3][1] = (s[0][0] * s[2][1] * s[3][2] + s[0][1] * s[2][2] * s[3][0] + s[0][2] * s[2][0] * s[3][1] - s[0][0] * s[2][2] * s[3][1] - s[0][1] * s[2][0] * s[3][2] - s[0][2] * s[2][1] * s[3][0]) / det;
		ret.m_element[0][2] = (s[0][1] * s[1][2] * s[3][3] + s[1][1] * s[3][2] * s[0][3] + s[3][1] * s[0][2] * s[1][3] - s[0][1] * s[3][2] * s[1][3] - s[1][1] * s[0][2] * s[3][3] - s[3][1] * s[1][2] * s[0][3]) / det;
		ret.m_element[1][2] = (s[0][0] * s[3][2] * s[1][3] + s[1][0] * s[0][2] * s[3][3] + s[3][0] * s[1][2] * s[0][3] - s[0][0] * s[1][2] * s[3][3] - s[1][0] * s[3][2] * s[0][3] - s[3][0] * s[0][2] * s[1][3]) / det;
		ret.m_element[2][2] = (s[0][0] * s[1][1] * s[3][3] + s[1][0] * s[3][1] * s[0][3] + s[3][0] * s[0][1] * s[1][3] - s[0][0] * s[3][1] * s[1][3] - s[1][0] * s[0][1] * s[3][3] - s[3][0] * s[1][1] * s[0][3]) / det;
		ret.m_element[3][2] = (s[0][0] * s[1][2] * s[3][1] + s[0][1] * s[1][0] * s[3][2] + s[0][2] * s[1][1] * s[3][0] - s[0][0] * s[1][1] * s[3][2] - s[0][1] * s[1][2] * s[3][0] - s[0][2] * s[1][0] * s[3][1]) / det;
		ret.m_element[0][3] = (s[0][1] * s[2][2] * s[1][3] + s[1][1] * s[0][2] * s[2][3] + s[2][1] * s[1][2] * s[0][3] - s[0][1] * s[1][2] * s[2][3] - s[1][1] * s[2][2] * s[0][3] - s[2][1] * s[0][2] * s[1][3]) / det;
		ret.m_element[1][3] = (s[0][0] * s[1][2] * s[2][3] + s[1][0] * s[2][2] * s[0][3] + s[2][0] * s[0][2] * s[1][3] - s[0][0] * s[2][2] * s[1][3] - s[1][0] * s[0][2] * s[2][3] - s[2][0] * s[1][2] * s[0][3]) / det;
		ret.m_element[2][3] = (s[0][0] * s[2][1] * s[1][3] + s[1][0] * s[0][1] * s[2][3] + s[2][0] * s[1][1] * s[0][3] - s[0][0] * s[1][1] * s[2][3] - s[1][0] * s[2][1] * s[0][3] - s[2][0] * s[0][1] * s[1][3]) / det;
		ret.m_element[3][3] = (s[0][0] * s[1][1] * s[2][2] + s[1][0] * s[2][1] * s[0][2] + s[2][0] * s[0][1] * s[1][2] - s[0][0] * s[2][1] * s[1][2] - s[1][0] * s[0][1] * s[2][2] - s[2][0] * s[1][1] * s[0][2]) / det;
	} else {
		memset(ret, 0, sizeof(_nzMatrix44<TYPE>));
	}

	return ret;
}

template<typename TYPE>
_nzMatrix44<TYPE> _nzMatrix44<TYPE>::transpose() const
{
	_nzMatrix44<TYPE> ret;
	ret.m_element[0][0] = m_element[0][0];
	ret.m_element[1][0] = m_element[0][1];
	ret.m_element[2][0] = m_element[0][2];
	ret.m_element[3][0] = m_element[0][3];
	ret.m_element[0][1] = m_element[1][0];
	ret.m_element[1][1] = m_element[1][1];
	ret.m_element[2][1] = m_element[1][2];
	ret.m_element[3][1] = m_element[1][3];
	ret.m_element[0][2] = m_element[2][0];
	ret.m_element[1][2] = m_element[2][1];
	ret.m_element[2][2] = m_element[2][2];
	ret.m_element[3][2] = m_element[2][3];
	ret.m_element[0][3] = m_element[3][0];
	ret.m_element[1][3] = m_element[3][1];
	ret.m_element[2][3] = m_element[3][2];
	ret.m_element[3][3] = m_element[3][3];
	return ret;
}

template<typename TYPE>
TYPE _nzMatrix44<TYPE>::determinant() const
{
	return
		m_element[0][0] * m_element[1][1] * m_element[2][2] * m_element[3][3] + m_element[0][0] * m_element[2][1] * m_element[3][2] * m_element[1][3] + m_element[0][0] * m_element[3][1] * m_element[1][2] * m_element[2][3] +
		m_element[1][0] * m_element[0][1] * m_element[3][2] * m_element[2][3] + m_element[1][0] * m_element[2][1] * m_element[0][2] * m_element[3][3] + m_element[1][0] * m_element[3][1] * m_element[2][2] * m_element[0][3] +
		m_element[2][0] * m_element[0][1] * m_element[1][2] * m_element[3][3] + m_element[2][0] * m_element[1][1] * m_element[3][2] * m_element[0][3] + m_element[2][0] * m_element[3][1] * m_element[0][2] * m_element[1][3] +
		m_element[3][0] * m_element[0][1] * m_element[2][2] * m_element[1][3] + m_element[3][0] * m_element[1][1] * m_element[0][2] * m_element[2][3] + m_element[3][0] * m_element[2][1] * m_element[1][2] * m_element[0][3] -
		m_element[0][0] * m_element[1][1] * m_element[3][2] * m_element[2][3] - m_element[0][0] * m_element[2][1] * m_element[1][2] * m_element[3][3] - m_element[0][0] * m_element[3][1] * m_element[2][2] * m_element[1][3] -
		m_element[1][0] * m_element[0][1] * m_element[2][2] * m_element[3][3] - m_element[1][0] * m_element[2][1] * m_element[3][2] * m_element[0][3] - m_element[1][0] * m_element[3][1] * m_element[0][2] * m_element[2][3] -
		m_element[2][0] * m_element[0][1] * m_element[3][2] * m_element[1][3] - m_element[2][0] * m_element[1][1] * m_element[0][2] * m_element[3][3] - m_element[2][0] * m_element[3][1] * m_element[1][2] * m_element[0][3] -
		m_element[3][0] * m_element[0][1] * m_element[1][2] * m_element[2][3] - m_element[3][0] * m_element[1][1] * m_element[2][2] * m_element[0][3] - m_element[3][0] * m_element[2][1] * m_element[0][2] * m_element[1][3];
}

template<typename TYPE>
_nzVector3<TYPE> _nzMatrix44<TYPE>::extractTranslate() const
{
	return _nzVector3<TYPE>(m_element[3][0], m_element[3][1], m_element[3][2]);
}

template<typename TYPE>
_nzVector3<TYPE> _nzMatrix44<TYPE>::extractScale() const
{
	_nzVector3<TYPE> x(m_element[0][0], m_element[1][0], m_element[2][0]);
	_nzVector3<TYPE> y(m_element[0][1], m_element[1][1], m_element[2][1]);
	_nzVector3<TYPE> z(m_element[0][2], m_element[1][2], m_element[2][2]);
	return _nzVector3<TYPE>(x.length(), y.length(), z.length());
}

/**
 * http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech52.html
 */
template<typename TYPE>
_nzQuaternion<TYPE> _nzMatrix44<TYPE>::extractQuaternion() const
{
	TYPE q0 = ( m_element[0][0] + m_element[1][1] + m_element[2][2] + 1) / 4,
		 q1 = ( m_element[0][0] - m_element[1][1] - m_element[2][2] + 1) / 4,
		 q2 = (-m_element[0][0] + m_element[1][1] - m_element[2][2] + 1) / 4,
		 q3 = (-m_element[0][0] - m_element[1][1] + m_element[2][2] + 1) / 4;
	q0 = (q0 > 0) ? q0 : 0;
	q1 = (q1 > 0) ? q1 : 0;
	q2 = (q2 > 0) ? q2 : 0;
	q3 = (q3 > 0) ? q3 : 0;
	q0 = (TYPE)sqrt((float)q0);
	q1 = (TYPE)sqrt((float)q1);
	q2 = (TYPE)sqrt((float)q2);
	q3 = (TYPE)sqrt((float)q3);
	if(q0 >= q1 && q0 >= q2 && q0 >= q3) {
		q1 *= NZ_MATRIX_SIGN(m_element[1][2] - m_element[2][1]);
		q2 *= NZ_MATRIX_SIGN(m_element[2][0] - m_element[0][2]);
		q3 *= NZ_MATRIX_SIGN(m_element[0][1] - m_element[1][0]);
	} else if(q1 >= q0 && q1 >= q2 && q1 >= q3) {
		q0 *= NZ_MATRIX_SIGN(m_element[1][2] - m_element[2][1]);
		q2 *= NZ_MATRIX_SIGN(m_element[0][1] + m_element[1][0]);
		q3 *= NZ_MATRIX_SIGN(m_element[2][0] + m_element[0][2]);
	} else if(q2 >= q0 && q2 >= q1 && q2 >= q3) {
		q0 *= NZ_MATRIX_SIGN(m_element[2][0] - m_element[0][2]);
		q1 *= NZ_MATRIX_SIGN(m_element[0][1] + m_element[1][0]);
		q3 *= NZ_MATRIX_SIGN(m_element[1][2] + m_element[2][1]);
	} else if(q3 >= q0 && q3 >= q1 && q3 >= q2) {
		q0 *= NZ_MATRIX_SIGN(m_element[0][1] - m_element[1][0]);
		q1 *= NZ_MATRIX_SIGN(m_element[0][2] + m_element[2][0]);
		q2 *= NZ_MATRIX_SIGN(m_element[1][2] + m_element[2][1]);
	} else {
		assert(0);
	}
	_nzQuaternion<TYPE> q(q0, q1, q2, q3);
	float norm = (float)q.norm();
	if(norm != 0) {
		q = q / norm;
	} else {
		assert(0);
	}
	return q;
}


template<typename TYPE>
const _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = mat1.m_element[c][r] + mat2.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat, float val)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] + val);
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			TYPE sum = 0;
			for(int i = 0; i < 4; i++){
				sum += mat1.m_element[i][r] * mat2.m_element[c][i];
			}
			ret.m_element[c][r] = sum;
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat, float val)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] * val);
		}
	}
	return ret;
}

template<typename TYPE>
const _nzVector4<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector4<TYPE> &vec)
{
	_nzVector4<TYPE> ret;
	ret.x = vec.x * mat.m_element[0][0] + vec.y * mat.m_element[1][0] + vec.z * mat.m_element[2][0] + vec.w * mat.m_element[3][0];
	ret.y = vec.x * mat.m_element[0][1] + vec.y * mat.m_element[1][1] + vec.z * mat.m_element[2][1] + vec.w * mat.m_element[3][1];
	ret.z = vec.x * mat.m_element[0][2] + vec.y * mat.m_element[1][2] + vec.z * mat.m_element[2][2] + vec.w * mat.m_element[3][2];
	ret.w = vec.x * mat.m_element[0][3] + vec.y * mat.m_element[1][3] + vec.z * mat.m_element[2][3] + vec.w * mat.m_element[3][3];
	return ret;
}

template<typename TYPE>
const _nzVector3<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector3<TYPE> &vec)
{
	_nzVector4<TYPE> vec4 = _nzVector4<TYPE>((TYPE)vec.x, (TYPE)vec.y, (TYPE)vec.z, 1);
	vec4 = mat * vec4;
	return _nzVector3<TYPE>(vec4.x / vec4.w, vec4.y / vec4.w, vec4.z / vec4.w);
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = -mat.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = mat1.m_element[c][r] - mat2.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat, float val)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] - val);
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator /(const _nzMatrix44<TYPE> &mat, float val)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] / val);
		}
	}
	return ret;
}

template<typename TYPE>
const _nzMatrix44<TYPE> operator /(float val, const _nzMatrix44<TYPE> &mat)
{
	_nzMatrix44<TYPE> ret;
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			ret.m_element[c][r] = (TYPE)(val / mat.m_element[c][r]);
		}
	}
	return ret;
}


#define REALIZE_TEMPLATE_NZ_MATRIX44(TYPE) \
	template _nzMatrix44<TYPE> _nzMatrix44<TYPE>::unit(); \
	template void _nzMatrix44<TYPE>::translate(TYPE x, TYPE y, TYPE z); \
	template void _nzMatrix44<TYPE>::scale(TYPE x, TYPE y, TYPE z); \
	template void _nzMatrix44<TYPE>::rotate(const _nzQuaternion<TYPE> &q); \
	template _nzMatrix44<TYPE> _nzMatrix44<TYPE>::invert() const; \
	template _nzMatrix44<TYPE> _nzMatrix44<TYPE>::transpose() const; \
	template TYPE _nzMatrix44<TYPE>::determinant() const; \
	template _nzVector3<TYPE> _nzMatrix44<TYPE>::extractTranslate() const; \
	template _nzVector3<TYPE> _nzMatrix44<TYPE>::extractScale() const; \
	template _nzQuaternion<TYPE> _nzMatrix44<TYPE>::extractQuaternion() const; \
	template const _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2); \
	template const _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat, float val); \
	template const _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2); \
	template const _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat, float val); \
	template const _nzVector4<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector4<TYPE> &vec); \
	template const _nzVector3<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector3<TYPE> &vec); \
	template const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat); \
	template const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2); \
	template const _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat, float val); \
	template const _nzMatrix44<TYPE> operator /(const _nzMatrix44<TYPE> &mat, float val); \
	template const _nzMatrix44<TYPE> operator /(float val, const _nzMatrix44<TYPE> &mat); \

REALIZE_TEMPLATE_NZ_MATRIX44(double)
REALIZE_TEMPLATE_NZ_MATRIX44(float)
REALIZE_TEMPLATE_NZ_MATRIX44(int)


/*******************************************
 *	nzMatrix33
 *******************************************/

template<typename TYPE>
_nzMatrix33<TYPE> _nzMatrix33<TYPE>::unit()
{
	_nzMatrix33<TYPE> ret;
	memset(&ret, 0, sizeof(_nzMatrix33<TYPE>));
	ret.m_element[0][0] = 1;
	ret.m_element[1][1] = 1;
	ret.m_element[2][2] = 1;
	return ret;
}

template<typename TYPE>
void _nzMatrix33<TYPE>::translate(TYPE x, TYPE y)
{
	_nzMatrix33<TYPE> mat(_nzMatrix33<TYPE>::unit());
	mat.m_element[2][0] = x;
	mat.m_element[2][1] = y;
	(*this) = mat * (*this);
}

template<typename TYPE>
void _nzMatrix33<TYPE>::scale(TYPE x, TYPE y)
{
	_nzMatrix33<TYPE> mat(_nzMatrix33<TYPE>::unit());
	mat.m_element[0][0] = x;
	mat.m_element[1][1] = y;
	(*this) = mat * (*this);
}

template<typename TYPE>
_nzMatrix33<TYPE> _nzMatrix33<TYPE>::invert() const
{
	_nzMatrix33<TYPE> ret;
	TYPE s[3][3];
	memcpy(s, m_element, 3 * 3 * sizeof(TYPE));

	TYPE det = determinant();
	if(0 != det){
		ret.m_element[0][0] = (s[1][1] * s[2][2] - s[2][1] * s[1][2]) / det;
		ret.m_element[1][0] = (s[2][0] * s[1][2] - s[1][0] * s[2][2]) / det;
		ret.m_element[2][0] = (s[1][0] * s[2][1] - s[2][0] * s[1][1]) / det;
		ret.m_element[0][1] = (s[2][1] * s[0][2] - s[0][1] * s[2][2]) / det;
		ret.m_element[1][1] = (s[0][0] * s[2][2] - s[2][0] * s[0][2]) / det;
		ret.m_element[2][1] = (s[2][0] * s[0][1] - s[0][0] * s[2][1]) / det;
		ret.m_element[0][2] = (s[0][1] * s[1][2] - s[1][1] * s[0][2]) / det;
		ret.m_element[1][2] = (s[1][0] * s[0][2] - s[0][0] * s[1][2]) / det;
		ret.m_element[2][2] = (s[0][0] * s[1][1] - s[1][0] * s[0][1]) / det;
	} else {
		memset(ret, 0, sizeof(_nzMatrix33<TYPE>));
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> _nzMatrix33<TYPE>::transpose() const
{
	_nzMatrix33<TYPE> ret;
	ret.m_element[0][0] = m_element[0][0];
	ret.m_element[1][0] = m_element[0][1];
	ret.m_element[2][0] = m_element[0][2];
	ret.m_element[0][1] = m_element[1][0];
	ret.m_element[1][1] = m_element[1][1];
	ret.m_element[2][1] = m_element[1][2];
	ret.m_element[0][2] = m_element[2][0];
	ret.m_element[1][2] = m_element[2][1];
	ret.m_element[2][2] = m_element[2][2];
	return ret;
}

template<typename TYPE>
TYPE _nzMatrix33<TYPE>::determinant() const
{
	return
		m_element[0][0] * m_element[1][1] * m_element[2][2] + m_element[0][1] * m_element[1][2] * m_element[2][0] + m_element[0][2] * m_element[1][0] * m_element[2][1] -
	    m_element[0][0] * m_element[1][2] * m_element[2][1] - m_element[0][2] * m_element[1][1] * m_element[2][0] - m_element[0][1] * m_element[1][0] * m_element[2][2];
}

template<typename TYPE>
_nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = mat1.m_element[c][r] + mat2.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat, float val)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] + val);
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			TYPE sum = 0;
			for(int i = 0; i < 3; i++){
				sum += mat1.m_element[i][r] * mat2.m_element[c][i];
			}
			ret.m_element[c][r] = sum;
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat, float val)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] * val);
		}
	}
	return ret;
}

template<typename TYPE>
_nzVector3<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector3<TYPE> &vec)
{
	_nzVector3<TYPE> ret;
	ret.x = vec.x * mat.m_element[0][0] + vec.y * mat.m_element[1][0] + vec.z * mat.m_element[2][0];
	ret.y = vec.x * mat.m_element[0][1] + vec.y * mat.m_element[1][1] + vec.z * mat.m_element[2][1];
	ret.z = vec.x * mat.m_element[0][2] + vec.y * mat.m_element[1][2] + vec.z * mat.m_element[2][2];
	return ret;
}

template<typename TYPE>
_nzVector2<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector2<TYPE> &vec)
{
	_nzVector3<TYPE> vec3 = _nzVector3<TYPE>(vec.x, vec.y, 1);
	vec3 = mat * vec3;
	return _nzVector2<TYPE>(vec3.x / vec3.z, vec3.y / vec3.z);
}

template<typename TYPE>
_nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = -mat.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = mat1.m_element[c][r] - mat2.m_element[c][r];
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat, float val)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] - val);
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator /(const _nzMatrix33<TYPE> &mat, float val)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = (TYPE)(mat.m_element[c][r] / val);
		}
	}
	return ret;
}

template<typename TYPE>
_nzMatrix33<TYPE> operator /(float val, const _nzMatrix33<TYPE> &mat)
{
	_nzMatrix33<TYPE> ret;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			ret.m_element[c][r] = (TYPE)(val / mat.m_element[c][r]);
		}
	}
	return ret;
}

#define REALIZE_TEMPLATE_NZ_MATRIX33(TYPE) \
	template _nzMatrix33<TYPE> _nzMatrix33<TYPE>::unit(); \
	template void _nzMatrix33<TYPE>::translate(TYPE x, TYPE y); \
	template void _nzMatrix33<TYPE>::scale(TYPE x, TYPE y); \
	template _nzMatrix33<TYPE> _nzMatrix33<TYPE>::invert() const; \
	template _nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2); \
	template _nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat, float val); \
	template _nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2); \
	template _nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat, float val); \
	template _nzVector3<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector3<TYPE> &vec); \
	template _nzVector2<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector2<TYPE> &vec); \
	template _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat); \
	template _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2); \
	template _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat, float val); \
	template _nzMatrix33<TYPE> operator /(const _nzMatrix33<TYPE> &mat, float val); \
	template _nzMatrix33<TYPE> operator /(float val, const _nzMatrix33<TYPE> &mat); \

REALIZE_TEMPLATE_NZ_MATRIX33(double)
REALIZE_TEMPLATE_NZ_MATRIX33(float)
REALIZE_TEMPLATE_NZ_MATRIX33(int)


/*******************************************
 *	nzMatrix22
 *******************************************/

template<typename TYPE>
_nzMatrix22<TYPE> _nzMatrix22<TYPE>::unit()
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = 1;
	ret.m_element[0][1] = 0;
	ret.m_element[1][0] = 0;
	ret.m_element[1][1] = 1;
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> _nzMatrix22<TYPE>::invert() const
{
	_nzMatrix22<TYPE> ret;
	TYPE s[2][2];
	memcpy(s, m_element, 2 * 2 * sizeof(TYPE));

	TYPE det = determinant();
	if(0 != det){
		ret.m_element[0][0] =  s[1][1] / det;
		ret.m_element[1][0] = -s[1][0] / det;
		ret.m_element[0][1] = -s[0][1] / det;
		ret.m_element[1][1] =  s[0][0] / det;
	} else {
		memset(ret, 0, sizeof(_nzMatrix22<TYPE>));
	}
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> _nzMatrix22<TYPE>::transpose() const
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = m_element[0][0];
	ret.m_element[1][0] = m_element[0][1];
	ret.m_element[0][1] = m_element[1][0];
	ret.m_element[1][1] = m_element[1][1];
	return ret;
}

template<typename TYPE>
TYPE _nzMatrix22<TYPE>::determinant() const
{
	return m_element[0][0] * m_element[1][1] - m_element[0][1] * m_element[1][0];
}

template<typename TYPE>
_nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = mat1.m_element[0][0] + mat2.m_element[0][0];
	ret.m_element[1][0] = mat1.m_element[1][0] + mat2.m_element[1][0];
	ret.m_element[0][1] = mat1.m_element[0][1] + mat2.m_element[0][1];
	ret.m_element[1][1] = mat1.m_element[1][1] + mat2.m_element[1][1];
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat, float val)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = (TYPE)(mat.m_element[0][0] + val);
	ret.m_element[1][0] = (TYPE)(mat.m_element[1][0] + val);
	ret.m_element[0][1] = (TYPE)(mat.m_element[0][1] + val);
	ret.m_element[1][1] = (TYPE)(mat.m_element[1][1] + val);
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = mat1.m_element[0][0] * mat2.m_element[0][0];
	ret.m_element[1][0] = mat1.m_element[1][0] * mat2.m_element[1][0];
	ret.m_element[0][1] = mat1.m_element[0][1] * mat2.m_element[0][1];
	ret.m_element[1][1] = mat1.m_element[1][1] * mat2.m_element[1][1];
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat, float val)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = (TYPE)(mat.m_element[0][0] * val);
	ret.m_element[1][0] = (TYPE)(mat.m_element[1][0] * val);
	ret.m_element[0][1] = (TYPE)(mat.m_element[0][1] * val);
	ret.m_element[1][1] = (TYPE)(mat.m_element[1][1] * val);
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = -mat.m_element[0][0];
	ret.m_element[1][0] = -mat.m_element[1][0];
	ret.m_element[0][1] = -mat.m_element[0][1];
	ret.m_element[1][1] = -mat.m_element[1][1];
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = mat1.m_element[0][0] - mat2.m_element[0][0];
	ret.m_element[1][0] = mat1.m_element[1][0] - mat2.m_element[1][0];
	ret.m_element[0][1] = mat1.m_element[0][1] - mat2.m_element[0][1];
	ret.m_element[1][1] = mat1.m_element[1][1] - mat2.m_element[1][1];
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat, float val)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = (TYPE)(mat.m_element[0][0] - val);
	ret.m_element[1][0] = (TYPE)(mat.m_element[1][0] - val);
	ret.m_element[0][1] = (TYPE)(mat.m_element[0][1] - val);
	ret.m_element[1][1] = (TYPE)(mat.m_element[1][1] - val);
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator /(const _nzMatrix22<TYPE> &mat, float val)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = (TYPE)(mat.m_element[0][0] / val);
	ret.m_element[1][0] = (TYPE)(mat.m_element[1][0] / val);
	ret.m_element[0][1] = (TYPE)(mat.m_element[0][1] / val);
	ret.m_element[1][1] = (TYPE)(mat.m_element[1][1] / val);
	return ret;
}

template<typename TYPE>
_nzMatrix22<TYPE> operator /(float val, const _nzMatrix22<TYPE> &mat)
{
	_nzMatrix22<TYPE> ret;
	ret.m_element[0][0] = (TYPE)(val / mat.m_element[0][0]);
	ret.m_element[1][0] = (TYPE)(val / mat.m_element[1][0]);
	ret.m_element[0][1] = (TYPE)(val / mat.m_element[0][1]);
	ret.m_element[1][1] = (TYPE)(val / mat.m_element[1][1]);
	return ret;
}

#define REALIZE_TEMPLATE_NZ_MATRIX22(TYPE) \
	template _nzMatrix22<TYPE> _nzMatrix22<TYPE>::unit(); \
	template _nzMatrix22<TYPE> _nzMatrix22<TYPE>::invert() const; \
	template _nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2); \
	template _nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat, float val); \
	template _nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2); \
	template _nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat, float val); \
	template _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat); \
	template _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2); \
	template _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat, float val); \
	template _nzMatrix22<TYPE> operator /(const _nzMatrix22<TYPE> &mat, float val); \
	template _nzMatrix22<TYPE> operator /(float val, const _nzMatrix22<TYPE> &mat); \

REALIZE_TEMPLATE_NZ_MATRIX22(double)
REALIZE_TEMPLATE_NZ_MATRIX22(float)
REALIZE_TEMPLATE_NZ_MATRIX22(int)

