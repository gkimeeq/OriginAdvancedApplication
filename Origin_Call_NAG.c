#include <Origin.h>
#include <OC_nag.h>  // ���ͷ�ļ���������ͨ�õ�NAGͷ�ļ�

/*
��ʾOrigin C����NAG������
���õ�NAG����Ϊ��
// void nag_simple_linear_regression (Nag_SumSquare mean, 
									//Integer n, const double x[], const double y[], const double wt[], 
									//double *a, double *b, double *a_serr, double *b_serr, 
									//double *rsq, double *rss, double *df, NagError *fail)

ʹ�÷�����
��Origin��Code Builder���򿪴��ļ��������뵽�����ռ䣬Ȼ����룬����Script Window���У�
Origin_Call_NAG();

������Ϊ��
�ع鳣���� a = 7.5982
�ع鳣����ı�׼�� a_serr = 6.6858
�ع�ϵ�� b = 7.0905
�ع�ϵ���ı�׼�� b_serr = 1.3224
�б�ϵ��R^2 rsq = 0.8273
�в�ƽ���� rss = 965.2454
���ɶ� df = 6.0000
*/								
void Origin_Call_NAG() {  // ����һ��Origin C����������ͨ������ֱ����Origin��Script Windowֱ������
	Nag_SumSquare mean = Nag_AboutMean;  // �ع��а���������
	int n;
	double *x, *y, *wt;
	double a, b, a_serr, b_serr, rsq, rss, df;
	NagError fail;
	
	// ׼������
	n = 8;
	x = (double *)malloc(sizeof(double) * n);
	y = (double *)malloc(sizeof(double) * n);
	wt = (double *)malloc(sizeof(double) * n);
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
	wt[0] = 1.0;
	wt[1] = 1.0;
	wt[2] = 1.0;
	wt[3] = 1.0;
	wt[4] = 1.0;
	wt[5] = 1.0;
	wt[6] = 1.0;
	wt[7] = 1.0;
	
	// ����NAG����
	nag_simple_linear_regression(mean, n, x, y, wt, &a, &b, &a_serr, &b_serr, 
								&rsq, &rss, &df, &fail); 
	// ��������
	//g02cac(mean, n, x, y, wt, &a, &b, &a_serr, &b_serr, &rsq, &rss, &df, &fail); 
	
	if (fail.code != NE_NOERROR) {  // �ع�ʧ��
		printf("����������nag_simple_linear_regression(g02cac)��\n%s\n", fail.message);
	} else {
		if (mean == Nag_AboutMean) {
			printf("\n�ع鳣���� a = %6.4f\n", a);
			printf("�ع鳣����ı�׼�� a_serr = %6.4f\n", a_serr);
		}
		printf("�ع�ϵ�� b = %6.4f\n", b);
		printf("�ع�ϵ���ı�׼�� b_serr = %6.4f\n", b_serr);
		printf("�б�ϵ��R^2 rsq = %6.4f\n", rsq);
		printf("�в�ƽ���� rss = %6.4f\n", rss);
		printf("���ɶ� df = %6.4f\n", df);
	}
	
	free(x);
	free(y);
	free(wt);
}