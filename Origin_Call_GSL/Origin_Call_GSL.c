#include <Origin.h>
#include "Origin_Call_GSL.h"  // ����ͷ�ļ�


/*
��ʾOrigin C����GSL������
���õ�GSL����Ϊ��
int gsl_fit_linear(const double * x, const size_t xstride, const double * y, const size_t ystride, 
	size_t n, double * c0, double * c1, double * cov00, double * cov01, double * cov11, double * sumsq);

������Ϊ��
�ع鳣����(�ؾ�) c0 = 7.5982
�ع�ϵ��(б��) c1 = 7.0905
������Э������� cov00 = 44.6994  cov01 = -6.5574  cov11 = 1.7486
�в�ƽ���� sumsq = 965.2454

*/
void Origin_Call_GSL() {  // ����һ��Origin C����������ͨ������ֱ����Origin��Script Windowֱ������
	size_t n;
	double *x, *y;
	size_t xstride, ystride;
	double c0, c1, cov00, cov01, cov11, sumsq;
	
	// ׼������
	n = 8;
	xstride = 1;
	ystride = 1;
	x = (double *)malloc(sizeof(double) * n);
	y = (double *)malloc(sizeof(double) * n);
	x[0] = 1.0;
	x[1] = 0.0;
	x[2] = 4.0;
	x[3] = 7.5;
	x[4] = 2.5;
	x[5] = 0.0;
	x[6] = 10.0;
	x[7] = 5.0;
	y[0] = 20.0;
	y[1] = 15.5;
	y[2] = 28.3;
	y[3] = 45.0;
	y[4] = 24.5;
	y[5] = 10.0;
	y[6] = 99.0;
	y[7] = 31.2;
	
	// ����GSL����
	gsl_fit_linear(x, 1, y, 1, n, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
	printf("\n�ع鳣����(�ؾ�) c0 = %6.4f\n", c0);
	printf("�ع�ϵ��(б��) c1 = %6.4f\n", c1);
	printf("������Э������� cov00 = %6.4f  cov01 = %6.4f  cov11 = %6.4f\n", cov00, cov01, cov11);
	printf("�в�ƽ���� sumsq = %6.4f\n", sumsq);
	
	free(x);
	free(y);
}