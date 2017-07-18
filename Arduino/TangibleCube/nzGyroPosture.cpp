
#include "nzGyroPosture.h"


//#include "gxBase/gxUtil.h"

#define ACC_FILTER_MOVING (.2f)
#define ACC_FILTER_STABLE (.5f)
#define GRAVITATIONAL_ACCELERATION (9.80665f)	// [m/sec^2]

//
static void _calculate_rotation(unsigned long dt_usec, const nzVector3f &gyro, const nzVector3f &acc, nzQuaternionf &quat);


nzGyroPosture::nzGyroPosture()
	: nzPosture(), m_accCount(0)
{
	//m_cs.init();
}

nzGyroPosture::~nzGyroPosture()
{
	//m_cs.fin();
}

void nzGyroPosture::inputGyroscope    (unsigned long timestampUsec, const nzVector3f &gyro)
{
	SensorData data = {timestampUsec, gyro};
	//m_cs.enter();
	m_gyroscopeQueue.push(data);
	//m_cs.leave();
}

void nzGyroPosture::inputAccelerometer(unsigned long timestampUsec, const nzVector3f &acc )
{
	SensorData data = {timestampUsec, acc};
	//m_cs.enter();
	m_accelerometerQueue.push(data);
	//m_cs.leave();
}

void nzGyroPosture::update()
{
	//m_cs.enter();
	if(m_gyroscopeQueue.size() > 1) {
		SensorData gyro = m_gyroscopeQueue.front() , acc = {0, nzVector3f(0, 0, 0)};
		m_gyroscopeQueue.pop();

		// ���Ԃ��������������g���̂ŁC�W���C���Z���T�̃f�[�^�͕����K�v
		while(!m_gyroscopeQueue.empty()) {
			// �W���C���Z���T�̒l���E��
			unsigned long dt_usec = m_gyroscopeQueue.front().timestamp - gyro.timestamp;

			// �����x�Z���T�̒l���E��
			while(!m_accelerometerQueue.empty() &&
				   m_accelerometerQueue.front().timestamp <= gyro.timestamp) {
				acc = m_accelerometerQueue.front();
				m_accelerometerQueue.pop();

				// �ŏ��̉����x�Z���T�̃f�[�^�ŁC-�d�͉����x�̃x�N�g�������ɏd�͕��������킹����
				if(2 == m_accCount++) {
					nzVector3f accNrm(acc.data * -1.0f), grvNrm(gravity());
					accNrm.normalize();
					grvNrm.normalize();
					nzVector3f crs = cross(accNrm, grvNrm);
					float      dt  = dot  (accNrm, grvNrm);
					quat.rotate(nzRad2Deg(acos(dt)), crs);
				}
			}
			_calculate_rotation(dt_usec, gyro.data, acc.data, nzPosture::quat);

			gyro = m_gyroscopeQueue.front();
			m_gyroscopeQueue.pop();
		}
		m_gyroscopeQueue.push(gyro);
	}
	//m_cs.leave();
}

nzVector3f nzGyroPosture::gravity()
{
	return rotateVectorByQuaternion(nzVector3f(0, 0, -GRAVITATIONAL_ACCELERATION), quat.invert());
}

void nzGyroPosture::clear()
{
	//m_cs.enter();
	while(!m_gyroscopeQueue.empty()) {
		m_gyroscopeQueue.pop();
	}
	while(!m_accelerometerQueue.empty()) {
		m_accelerometerQueue.pop();
	}
	quat  = nzQuaternionf::identity();
	trans = nzVector3f(0, 0 ,0);
	scale = nzVector3f(1.0f, 1.0f, 1.0f);
	m_accCount = 0;
	//m_cs.leave();
}

static bool _isStable(const nzVector3f &gyro, float angleVelThres)
{
	bool ret = false;
	if ( gyro.length() < angleVelThres) {
		ret = true;
	}
	return ret;
}

static void _calculate_rotation(unsigned long dt_usec, const nzVector3f &gyro, const nzVector3f &acc, nzQuaternionf &quat)
{
	double norm, pow2;
	double s1 = 0, s2 = 0, s3 = 0, s4 = 0;
	double qDot1, qDot2, qDot3, qDot4;
	double ax = acc.x, ay = acc.y, az = acc.z;
	nzVector3f gyro_rad = gyro * 3.14159f / 180.0f;
	
	// Auxiliary variables to avoid repeated arithmetic
	double q1 = quat.r, q2 = quat.i.x, q3 = quat.i.y, q4 = quat.i.z;   // short name local variable for readability
	double _2q1 = 2. * quat.r;
	double _2q2 = 2. * q2;
	double _2q3 = 2. * q3;
	double _2q4 = 2. * q4;
	double _4q1 = 4. * quat.r;
	double _4q2 = 4. * q2;
	double _4q3 = 4. * q3;
	double _8q2 = 8. * q2;
	double _8q3 = 8. * q3;
	double q1q1 = quat.r * quat.r;
	double q2q2 = q2 * q2;
	double q3q3 = q3 * q3;
	double q4q4 = q4 * q4;
	double beta = _isStable(gyro, 2.0f) ? ACC_FILTER_STABLE : ACC_FILTER_MOVING;

	// Normalise accelerometer measurement
	norm = acc.length();
	if (norm > DBL_EPSILON) {
		norm = 1 / norm;        // use reciprocal for division
		ax *= norm;
		ay *= norm;
		az *= norm;

		// Gradient decent algorithm corrective step
		s1 = _4q1 * q3q3 + _2q3 * ax + _4q1 * q2q2 - _2q2 * ay;
		s2 = _4q2 * q4q4 - _2q4 * ax + 4. * q1q1 * q2 - _2q1 * ay - _4q2 + _8q2 * q2q2 + _8q2 * q3q3 + _4q2 * az;
		s3 = 4. * q1q1 * q3 + _2q1 * ax + _4q3 * q4q4 - _2q4 * ay - _4q3 + _8q3 * q2q2 + _8q3 * q3q3 + _4q3 * az;
		s4 = 4. * q2q2 * q4 - _2q2 * ax + 4. * q3q3 * q4 - _2q3 * ay;
		pow2 = s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4;
		if (pow2 > DBL_EPSILON) {
			norm = 1. / sqrt(pow2);    // normalise step magnitude			s1 *= norm;
			s2 *= norm;
			s3 *= norm;
			s4 *= norm;
		}
	}

	// Compute rate of difference of mQuaternion
	qDot1 = 0.5 * (-q2 * gyro_rad.x - q3 * gyro_rad.y - q4 * gyro_rad.z) - beta * s1;
	qDot2 = 0.5 * ( q1 * gyro_rad.x - q4 * gyro_rad.y + q3 * gyro_rad.z) - beta * s2;
	qDot3 = 0.5 * ( q4 * gyro_rad.x + q1 * gyro_rad.y - q2 * gyro_rad.z) - beta * s3;
	qDot4 = 0.5 * (-q3 * gyro_rad.x + q2 * gyro_rad.y + q1 * gyro_rad.z) - beta * s4;

	// Integrate to yield mQuaternion
	double dt_sec = dt_usec * .000001;
	q1 += qDot1 * dt_sec;
	q2 += qDot2 * dt_sec;
	q3 += qDot3 * dt_sec;
	q4 += qDot4 * dt_sec;

	pow2 = q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4;
	if (pow2 < DBL_EPSILON) {
		return; // handle NaN
	}

	norm = 1. / sqrt(pow2);    // normalise quaternion
	quat.r   = (float)(q1 * norm);
	quat.i.x = (float)(q2 * norm);
	quat.i.y = (float)(q3 * norm);
	quat.i.z = (float)(q4 * norm);
}



