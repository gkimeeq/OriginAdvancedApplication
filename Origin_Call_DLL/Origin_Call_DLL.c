#include <Origin.h>
#include "Origin_Call_DLL.h"  // ����ͷ�ļ�

/*
��ʾOrigin C����DLL������

���Ϊ��
Բ����Pi = 3.141593
Բ��Ϊ��5.000000, 6.000000�����뾶Ϊ10.000000
*/
void Origin_Call_DLL() {  // ����һ��Origin C����������ͨ������ֱ����Origin��Script Windowֱ������
	double dPi = GetPi();  // ����DLL�еĺ�����������һ��ֵ
	printf("Բ����Pi = %.6f\n", dPi);
	Circle circle;
	GetCircle(&circle);  // ����DLL�еĺ�����ͨ��ָ�������ֵ��Origin C��֧��DLL�з��ؽṹ��
	printf("Բ��Ϊ��%.6f, %.6f�����뾶Ϊ%.6f\n", circle.cx, circle.cy, circle.radius);
}