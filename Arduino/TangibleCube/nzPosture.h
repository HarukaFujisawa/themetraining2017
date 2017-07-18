
#ifndef NZ_POSTURE_H
#define NZ_POSTURE_H

#include "nzVector.h"
#include "nzMatrix.h"
#include "nzQuaternion.h"

/**
 *	@addtogroup Graphics_eXperimental
 *	@{
 *	@file		nzPosture.h
 *	@author		NARITA Tomoya
 *
 *	@warning
 *	Copyright(C) 2010 Sony Corporation<br>
 *	All Rights Reserved.<br>
 *
 */

#define nzRad2Deg(R) (R * 3.14159f / 180.0f)
#define nzDeg2Rad(R) (R * 180.0f / 3.14159f)

typedef struct _nzPosture{
	_nzPosture() : trans(0, 0, 0), scale(1.0f, 1.0f, 1.0f), quat(nzQuaternionf::unit()) {}
	~_nzPosture() {}

	nzVector3f trans;	///< �ｿｽﾚ難ｿｽ�ｿｽv�ｿｽf
	nzVector3f scale;	///< �ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ�ｿｽv�ｿｽf
	nzQuaternionf quat;	///< �ｿｽ�ｿｽ]�ｿｽv�ｿｽf

	nzMatrix44f calcMatrix();
} nzPosture;

/**
 *	float�ｿｽﾆの擾ｿｽZ�ｿｽI�ｿｽy�ｿｽ�ｿｽ�ｿｽ[�ｿｽ^
 */
inline nzPosture operator *(const nzPosture &pos, float f) {
	nzPosture ret;
	ret.trans = pos.trans * f;
	ret.scale = pos.scale * f;
	ret.quat  = pos.quat  * f;
	return ret;
}

/**
 *	float�ｿｽﾆの擾ｿｽZ�ｿｽI�ｿｽy�ｿｽ�ｿｽ�ｿｽ[�ｿｽ^
 */
inline nzPosture operator *(float f, const nzPosture &pos) {
	return pos * f;
}

/**
 *	�ｿｽ�ｿｽ�ｿｽZ�ｿｽI�ｿｽy�ｿｽ�ｿｽ�ｿｽ[�ｿｽ^
 */
inline nzPosture operator +(const nzPosture &pos1, const nzPosture &pos2) {
	nzPosture ret;
	ret.trans = pos1.trans + pos2.trans;
	ret.scale = pos1.scale + pos2.scale;
	ret.quat  = pos1.quat  + pos2.quat ;
	return ret;
}

/**	@} */
#endif // NZ_POSTURE_H


