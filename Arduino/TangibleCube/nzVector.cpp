
#include "nzVector.h"

template<typename TYPE>
bool nzIntersection(const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2, const _nzVector3<TYPE> &ptPlane1, const _nzVector3<TYPE> &ptPlane2, const _nzVector3<TYPE> &ptPlane3, _nzVector3<TYPE> &ptIntersection)
{
	bool err = false;

	_nzVector3<TYPE> planeNormal(cross(ptPlane2 - ptPlane1, ptPlane3 - ptPlane1));
	_nzVector3<TYPE> lineDirection(ptLine2 - ptLine1);
	TYPE d = dot(planeNormal, lineDirection);

	if(0 != d){
		ptIntersection = ptLine1 - (dot(planeNormal, ptLine1 - ptPlane1) / d) * lineDirection;
		err = true;
	}

	return err;
}

template<typename TYPE>
bool nzIntersection(const _nzVector2<TYPE> &pt1Line1, const _nzVector2<TYPE> &pt2Line1, const _nzVector2<TYPE> &pt1Line2, const _nzVector2<TYPE> &pt2Line2, _nzVector2<TYPE> &ptIntersection)
{
	bool err = false;
	TYPE a1 = pt2Line1.y - pt1Line1.y;
	TYPE b1 = pt1Line1.x - pt2Line1.x;
	TYPE d1 = a1 * pt1Line1.x + b1 * pt1Line1.y;
	TYPE a2 = pt2Line2.y - pt1Line2.y;
	TYPE b2 = pt1Line2.x - pt2Line2.x;
	TYPE d2 = a2 * pt1Line2.x + b2 * pt1Line2.y;
	TYPE denom = a1 * b2 - a2 * b1;

	if(denom != 0){
		ptIntersection = _nzVector2<TYPE>((b2 * d1 - b1 * d2) / denom, (a1 * d2 - a2 * d1) / denom);
		err = true;
	}

	return err;
}

template <typename TYPE>
TYPE nzDistance(const _nzVector3<TYPE> &point, const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2)
{
	TYPE ret = 0;

	TYPE f1 = dot(point - ptLine1, ptLine2 - ptLine1);
	TYPE f2 = dot(ptLine2 - ptLine1, ptLine2 - ptLine1);

	if(f1 <= 0){
		ret = (point - ptLine1).length();
	} else if(f1 >= f2){
		ret = (point - ptLine2).length();
	} else {
		TYPE d1 = (point - ptLine1).length();
		TYPE d2 = f1 / (ptLine2 - ptLine1).length();
		ret = (TYPE)sqrt((float)(d1 * d1 - d2 * d2));
	}

	return ret;
}

// �ｿｽe�ｿｽ�ｿｽ�ｿｽv�ｿｽ�ｿｽ�ｿｽ[�ｿｽg�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽp�ｿｽ}�ｿｽN�ｿｽ�ｿｽ.
#define REALIZE_TEMPLATE_NZ_VECTOR(TYPE) \
	template bool nzIntersection(const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2, const _nzVector3<TYPE> &ptPlane1, const _nzVector3<TYPE> &ptPlane2, const _nzVector3<TYPE> &ptPlane3, _nzVector3<TYPE> &ptIntersection); \
	template bool nzIntersection(const _nzVector2<TYPE> &pt1Line1, const _nzVector2<TYPE> &pt2Line1, const _nzVector2<TYPE> &pt1Line2, const _nzVector2<TYPE> &pt2Line2, _nzVector2<TYPE> &ptIntersection); \
	template TYPE nzDistance(const _nzVector3<TYPE> &point, const _nzVector3<TYPE> &ptLine1, const _nzVector3<TYPE> &ptLine2); \

REALIZE_TEMPLATE_NZ_VECTOR(double)
REALIZE_TEMPLATE_NZ_VECTOR(float)
REALIZE_TEMPLATE_NZ_VECTOR(int)


