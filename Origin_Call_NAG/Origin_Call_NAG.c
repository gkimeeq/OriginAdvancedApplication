#include <Origin.h>
#include <OC_nag.h>  // 这个头文件包括所有通用的NAG头文件

/*
演示Origin C调用NAG函数。
调用的NAG函数为：
// void nag_simple_linear_regression (Nag_SumSquare mean, 
									//Integer n, const double x[], const double y[], const double wt[], 
									//double *a, double *b, double *a_serr, double *b_serr, 
									//double *rsq, double *rss, double *df, NagError *fail)

使用方法：
在Origin的Code Builder，打开此文件，并加入到工作空间，然后编译，再在Script Window运行：
Origin_Call_NAG();

输出结果为：
回归常数项 a = 7.5982
回归常数项的标准差 a_serr = 6.6858
回归系数 b = 7.0905
回归系数的标准差 b_serr = 1.3224
判别系数R^2 rsq = 0.8273
残差平方和 rss = 965.2454
自由度 df = 6.0000
*/								
void Origin_Call_NAG() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	Nag_SumSquare mean = Nag_AboutMean;  // 回归中包括常数项
	int n;
	double *x, *y, *wt;
	double a, b, a_serr, b_serr, rsq, rss, df;
	NagError fail;
	
	// 准备数据
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
	
	// 调用NAG函数
	nag_simple_linear_regression(mean, n, x, y, wt, &a, &b, &a_serr, &b_serr, 
								&rsq, &rss, &df, &fail); 
	// 或者这样
	//g02cac(mean, n, x, y, wt, &a, &b, &a_serr, &b_serr, &rsq, &rss, &df, &fail); 
	
	if (fail.code != NE_NOERROR) {  // 回归失败
		printf("错误来自于nag_simple_linear_regression(g02cac)。\n%s\n", fail.message);
	} else {
		if (mean == Nag_AboutMean) {
			printf("\n回归常数项 a = %6.4f\n", a);
			printf("回归常数项的标准差 a_serr = %6.4f\n", a_serr);
		}
		printf("回归系数 b = %6.4f\n", b);
		printf("回归系数的标准差 b_serr = %6.4f\n", b_serr);
		printf("判别系数R^2 rsq = %6.4f\n", rsq);
		printf("残差平方和 rss = %6.4f\n", rss);
		printf("自由度 df = %6.4f\n", df);
	}
	
	free(x);
	free(y);
	free(wt);
}