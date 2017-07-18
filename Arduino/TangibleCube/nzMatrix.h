
#ifndef NZ_MATRIX_H
#define NZ_MATRIX_H

#include "nzVector.h"
#include "nzQuaternion.h"

/**
 *	@addtogroup Graphics_eXperimental
 *	@{
 *	@file		nzMatrix.h
 *	@author		NARITA Tomoya
 *
 *	@warning
 *	Copyright(C) 2010 Sony Corporation<br>
 *	All Rights Reserved.<br>
 *
 */

template<typename TYPE> struct _nzQuaternion;


/*******************************************
 *	nzMatrix44
 *******************************************/
template<typename TYPE> struct _nzMatrix44{
	TYPE m_element[4][4];

	/**
	 *	TYPE[16]�ｿｽﾆゑｿｽ�ｿｽﾄの会ｿｽ�ｿｽﾟのゑｿｽ�ｿｽ�ｿｽ
	 */
	operator TYPE *(){
		return &m_element[0][0];
	}

	/**
	 *	�ｿｽe�ｿｽv�ｿｽf�ｿｽﾖのア�ｿｽN�ｿｽZ�ｿｽX r:�ｿｽs, c:�ｿｽ�ｿｽ
	 */
	TYPE &element(int r, int c){
		return m_element[c][r];
	}

	/**
	 *	�ｿｽP�ｿｽﾊ行�ｿｽ�ｿｽ
	 */
	static struct _nzMatrix44<TYPE> unit();

	/**
	 *	�ｿｽﾚ難ｿｽ
	 */
	void translate(TYPE x, TYPE y, TYPE z);

	/**
	 *	�ｿｽﾚ難ｿｽ
	 */
	void translate(const nzVector3f &vec) {translate(vec.x, vec.y, vec.z);}

	/**
	 *	�ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽO
	 */
	void scale(TYPE x, TYPE y, TYPE z);

	/**
	 *	�ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽO
	 */
	void scale(const _nzVector3<TYPE> &vec) {scale(vec.x, vec.y, vec.z);}

	/**
	 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽ�ｿｽ]
	 */
	void rotate(const _nzQuaternion<TYPE> &q);

	/**
	 *	�ｿｽt�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix44 invert() const;

	/**
	 *	�ｿｽ]�ｿｽu�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix44 transpose() const;

	/**
	 *	�ｿｽs�ｿｽｮゑｿｽﾔゑｿｽ
	 */
	TYPE determinant() const;

	/**
	 *	�ｿｽﾚ難ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ鰹o�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	_nzVector3<TYPE> extractTranslate() const;

	/**
	 *	�ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽO�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ鰹o�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	_nzVector3<TYPE> extractScale() const;

	/**
	 *	�ｿｽN�ｿｽH�ｿｽ[�ｿｽ^�ｿｽj�ｿｽI�ｿｽ�ｿｽ�ｿｽ鰹o�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	 */
	_nzQuaternion<TYPE> extractQuaternion() const;
};

typedef struct _nzMatrix44<double> nzMatrix44d;
typedef struct _nzMatrix44<float>  nzMatrix44f;
typedef struct _nzMatrix44<int>    nzMatrix44i;


/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const        _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2);
template<typename TYPE> const        _nzMatrix44<TYPE> operator +(const _nzMatrix44<TYPE> &mat, float val);
template<typename TYPE> const inline _nzMatrix44<TYPE> operator +(float val, const nzMatrix44f &mat) {return mat + val;}

/**
 *	�ｿｽ�ｿｽZ
 */
template<typename TYPE> const        _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2);
template<typename TYPE> const        _nzMatrix44<TYPE> operator *(const _nzMatrix44<TYPE> &mat, float val);
template<typename TYPE> const inline _nzMatrix44<TYPE> operator *(float val, const _nzMatrix44<TYPE> &mat) {return mat * val;}
template<typename TYPE> const        _nzVector4<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector4<TYPE> &vec);
template<typename TYPE> const        _nzVector3<TYPE> operator *(const _nzMatrix44<TYPE> &mat, const _nzVector3<TYPE> &vec);
template<typename TYPE> const inline _nzVector4<TYPE> operator *(const _nzVector4<TYPE> &vec, const _nzMatrix44<TYPE> &mat) {return mat * vec;}
template<typename TYPE> const inline _nzVector3<TYPE> operator *(const _nzVector3<TYPE> &vec, const _nzMatrix44<TYPE> &mat) {return mat * vec;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const        _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat1);
template<typename TYPE> const        _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat1, const _nzMatrix44<TYPE> &mat2);
template<typename TYPE> const        _nzMatrix44<TYPE> operator -(const _nzMatrix44<TYPE> &mat, float val);
template<typename TYPE> const inline _nzMatrix44<TYPE> operator -(float val, const _nzMatrix44<TYPE> &mat) {return -mat + val;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE> const _nzMatrix44<TYPE> operator /(const _nzMatrix44<TYPE> &mat, float val);
template<typename TYPE> const _nzMatrix44<TYPE> operator /(float val, const _nzMatrix44<TYPE> &mat);


/*******************************************
 *	nzMatrix33
 *******************************************/
template<typename TYPE> struct _nzMatrix33{
	TYPE m_element[3][3];

	/**
	 *	TYPE[9]�ｿｽﾆゑｿｽ�ｿｽﾄの会ｿｽ�ｿｽﾟのゑｿｽ�ｿｽ�ｿｽ
	 */
	operator TYPE *(){
		return &m_element[0][0];
	}

	/**
	 *	�ｿｽe�ｿｽv�ｿｽf�ｿｽﾖのア�ｿｽN�ｿｽZ�ｿｽX r:�ｿｽs, c:�ｿｽ�ｿｽ
	 */
	TYPE &element(int r, int c){
		return m_element[c][r];
	}

	/**
	 *	�ｿｽP�ｿｽﾊ行�ｿｽ�ｿｽ
	 */
	static struct _nzMatrix33 unit();

	/**
	 *	�ｿｽﾚ難ｿｽ
	 */
	void translate(TYPE x, TYPE y);

	/**
	 *	�ｿｽﾚ難ｿｽ
	 */
	void translate(const _nzVector2<TYPE> &vec) {translate(vec.x, vec.y);}

	/**
	 *	�ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽO
	 */
	void scale(TYPE x, TYPE y);

	/**
	 *	�ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽO
	 */
	void scale(const _nzVector2<TYPE> &vec) {scale(vec.x, vec.y);}

	/**
	 *	�ｿｽt�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix33 invert() const;

	/**
	 *	�ｿｽ]�ｿｽu�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix33 transpose() const;

	/**
	 *	�ｿｽs�ｿｽｮゑｿｽﾔゑｿｽ
	 */
	TYPE determinant() const;
};

typedef struct _nzMatrix33<double> nzMatrix33d;
typedef struct _nzMatrix33<float>  nzMatrix33f;
typedef struct _nzMatrix33<int>    nzMatrix33i;

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2);
template<typename TYPE>        _nzMatrix33<TYPE> operator +(const _nzMatrix33<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix33<TYPE> operator +(float val, const _nzMatrix33<TYPE> &mat) {return mat + val;}

/**
 *	�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2);
template<typename TYPE>        _nzMatrix33<TYPE> operator *(const _nzMatrix33<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix33<TYPE> operator *(float val, const _nzMatrix33<TYPE> &mat) {return mat * val;}
template<typename TYPE>        _nzVector3<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector3<TYPE> &vec);
template<typename TYPE>        _nzVector2<TYPE> operator *(const _nzMatrix33<TYPE> &mat, const _nzVector2<TYPE> &vec);
template<typename TYPE> inline _nzVector3<TYPE> operator *(const _nzVector3<TYPE> &vec, const _nzMatrix33<TYPE> &mat) {return mat * vec;}
template<typename TYPE> inline _nzVector2<TYPE> operator *(const _nzVector2<TYPE> &vec, const _nzMatrix33<TYPE> &mat) {return mat * vec;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat1);
template<typename TYPE>        _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat1, const _nzMatrix33<TYPE> &mat2);
template<typename TYPE>        _nzMatrix33<TYPE> operator -(const _nzMatrix33<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix33<TYPE> operator -(float val, const _nzMatrix33<TYPE> &mat) {return -mat + val;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix33<TYPE> operator /(const _nzMatrix33<TYPE> &mat, float val);
template<typename TYPE>        _nzMatrix33<TYPE> operator /(float val, const _nzMatrix33<TYPE> &mat);


/*******************************************
 *	nzMatrix22
 *******************************************/
template<typename TYPE> struct _nzMatrix22{
	TYPE m_element[2][2];

	/**
	 *	TYPE[4]�ｿｽﾆゑｿｽ�ｿｽﾄの会ｿｽ�ｿｽﾟのゑｿｽ�ｿｽ�ｿｽ
	 */
	operator TYPE *(){
		return &m_element[0][0];
	}

	/**
	 *	�ｿｽe�ｿｽv�ｿｽf�ｿｽﾖのア�ｿｽN�ｿｽZ�ｿｽX r:�ｿｽs, c:�ｿｽ�ｿｽ
	 */
	TYPE &element(int r, int c){
		return m_element[c][r];
	}

	/**
	 *	�ｿｽP�ｿｽﾊ行�ｿｽ�ｿｽ
	 */
	static struct _nzMatrix22 unit();

	/**
	 *	�ｿｽt�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix22 invert() const;

	/**
	 *	�ｿｽ]�ｿｽu�ｿｽs�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	struct _nzMatrix22 transpose() const;

	/**
	 *	�ｿｽs�ｿｽｮゑｿｽﾔゑｿｽ
	 */
	TYPE determinant() const;
};

typedef struct _nzMatrix22<double> nzMatrix22d;
typedef struct _nzMatrix22<float>  nzMatrix22f;
typedef struct _nzMatrix22<int>    nzMatrix22i;

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2);
template<typename TYPE>        _nzMatrix22<TYPE> operator +(const _nzMatrix22<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix22<TYPE> operator +(float val, const _nzMatrix22<TYPE> &mat) {return mat + val;}

/**
 *	�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2);
template<typename TYPE>        _nzMatrix22<TYPE> operator *(const _nzMatrix22<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix22<TYPE> operator *(float val, const _nzMatrix22<TYPE> &mat) {return mat * val;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat1);
template<typename TYPE>        _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat1, const _nzMatrix22<TYPE> &mat2);
template<typename TYPE>        _nzMatrix22<TYPE> operator -(const _nzMatrix22<TYPE> &mat, float val);
template<typename TYPE> inline _nzMatrix22<TYPE> operator -(float val, const _nzMatrix22<TYPE> &mat) {return -mat + val;}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ
 */
template<typename TYPE>        _nzMatrix22<TYPE> operator /(const _nzMatrix22<TYPE> &mat, float val);
template<typename TYPE>        _nzMatrix22<TYPE> operator /(float val, const _nzMatrix22<TYPE> &mat);


/**	@} */
#endif // NZ_MATRIX_H

