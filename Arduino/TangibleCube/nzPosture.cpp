
#include "nzPosture.h"

nzMatrix44f nzPosture::calcMatrix()
{
	nzMatrix44f rMat(nzMatrix44f::unit()), sMat(nzMatrix44f::unit()), tMat(nzMatrix44f::unit());
	sMat.scale(scale);
	rMat.rotate(quat);
	tMat.translate(trans);
	return tMat * rMat * sMat;	// �ｿｽX�ｿｽP�ｿｽ[�ｿｽ�ｿｽ �ｿｽ�ｿｽ �ｿｽ�ｿｽ] �ｿｽ�ｿｽ �ｿｽﾚ難ｿｽ�ｿｽﾌ擾ｿｽ�ｿｽﾅ変奇ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
}


