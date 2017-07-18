/**
 *	@addtogroup Graphics_eXperimental
 *	@{
 *	@file		gxGyroPosture.h
 *	@author		NARITA Tomoya
 *
 *	@warning
 *	Copyright(C) 2015 Sony Corporation<br>
 *	All Rights Reserved.<br>
 *
 */

#ifndef NZ_GYRO_POSTURE_H
#define NZ_GYRO_POSTURE_H

#include "nzPosture.h"
#include <float.h>
//#include "UxnUtil/UxnCriticalSection.h"
#include <queue>

class nzGyroPosture : public nzPosture
{
public:
	/**
	 *	�ｿｽR�ｿｽ�ｿｽ�ｿｽX�ｿｽg�ｿｽ�ｿｽ�ｿｽN�ｿｽ^
	 */
	nzGyroPosture();

	/**
	 *	�ｿｽf�ｿｽX�ｿｽg�ｿｽ�ｿｽ�ｿｽN�ｿｽ^
	 */
	virtual ~nzGyroPosture();

	/**
	 *	�ｿｽW�ｿｽ�ｿｽ�ｿｽC�ｿｽ�ｿｽ�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌデ�ｿｽ[�ｿｽ^�ｿｽ謫ｾ�ｿｽ�ｿｽ�ｿｽﾉ難ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾜゑｿｽ�ｿｽB
	 *	@param timestampUsec	�ｿｽW�ｿｽ�ｿｽ�ｿｽC�ｿｽ�ｿｽ�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽﾌタ�ｿｽC�ｿｽ�ｿｽ�ｿｽX�ｿｽ^�ｿｽ�ｿｽ�ｿｽv[�ｿｽ�ｿｽsec]
	 *	@param gyro				�ｿｽW�ｿｽ�ｿｽ�ｿｽC�ｿｽ�ｿｽ�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽﾌ角�ｿｽ�ｿｽ�ｿｽx�ｿｽ�ｿｽ�ｿｽ [deg/sec]
	 */
	void inputGyroscope    (unsigned long timestampUsec, const nzVector3f &gyro);

	/**
	 *	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽx�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌデ�ｿｽ[�ｿｽ^�ｿｽ謫ｾ�ｿｽ�ｿｽ�ｿｽﾉ難ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾜゑｿｽ�ｿｽB
	 *	@param timestampUsec	�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽx�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽﾌタ�ｿｽC�ｿｽ�ｿｽ�ｿｽX�ｿｽ^�ｿｽ�ｿｽ�ｿｽv[�ｿｽ�ｿｽsec]
	 *	@param gyro				�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽx�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽﾌ会ｿｽ�ｿｽ�ｿｽ�ｿｽx�ｿｽ�ｿｽ�ｿｽ [m/sec^2]
	 */
	void inputAccelerometer(unsigned long timestampUsec, const nzVector3f &acc );
	//void inputMagneticField(unsigned long timestampUsec, const nzVector3f &mag );

	/**
	 *	input�ｿｽ�ｿｽ�ｿｽ黷ｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽｩゑｿｽCgxPosture�ｿｽﾌ値�ｿｽ�ｿｽ�ｿｽX�ｿｽV�ｿｽ�ｿｽ�ｿｽﾜゑｿｽ�ｿｽB
	 *	�ｿｽﾅ抵ｿｽ�ｿｽ�ｿｽQ�ｿｽﾂのジ�ｿｽ�ｿｽ�ｿｽC�ｿｽ�ｿｽ�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽﾌデ�ｿｽ[�ｿｽ^�ｿｽ�ｿｽK�ｿｽv�ｿｽﾆゑｿｽ�ｿｽﾜゑｿｽ�ｿｽB
	 */
	virtual void update();

	/**
	 *	�ｿｽZ�ｿｽ�ｿｽ�ｿｽT�ｿｽn�ｿｽ�ｿｽ�ｿｽ迪ｩ�ｿｽ�ｿｽ�ｿｽd�ｿｽﾍ包ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾔゑｿｽ
	 */
	nzVector3f gravity();

	/**
	 *
	 */
	void clear();

private:
	typedef struct _SensorData {
		unsigned long timestamp;
		nzVector3f data;
	} SensorData;

	std::queue<SensorData> m_accelerometerQueue;
	std::queue<SensorData> m_gyroscopeQueue;
	//std::queue<SensorData> m_magneticFieldQueue;

	unsigned int       m_accCount;
	//UxnCriticalSection m_cs;
};

#endif // NZ_GYRO_POSTURE_H
/**	@} */


