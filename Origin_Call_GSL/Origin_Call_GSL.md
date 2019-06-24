GSL （GNU Scientific Library, [http://www.gnu.org/software/gsl/](http://www.gnu.org/software/gsl/)）是C、C++的数值算法库，提供了范围宽广的数学程序，包括随机数字生成器、数理方程与特殊函数、最小二乘法拟合等。最重要的是GSL是基于GNU General Public License的，可以免费使用。

Origin C中调用GSL，要用到两个`dll`，`libgsl.dll`和`libgslcblas.dll`。为了简单起见，直接用现成的`dll`，可以从[http://gnuwin32.sourceforge.net/packages/gsl.htm](http://gnuwin32.sourceforge.net/packages/gsl.htm)下载。找到网页上`Download`的`Binaries`，下载`Zip`，解压后在`bin`文件夹可以找到对应的两个`dll`。注意这两个`dll`对应的GSL版本是`1.8`，而且是32位的，要用更高版本或64位版本，需要自行编译。

这里演示的是调用GSL的线性拟合的函数，这个函数的原型如下：
```
int gsl_fit_linear(const double * x, const size_t xstride, const double * y, const size_t ystride, size_t n, double * c0, double * c1, double * cov00, double * cov01, double * cov11, double * sumsq)
```

关于这个函数的说明，可以参考GSL关于线性回归的文档[http://www.gnu.org/software/gsl/doc/html/lls.html#linear-regression-with-a-constant-term](http://www.gnu.org/software/gsl/doc/html/lls.html#linear-regression-with-a-constant-term)。

1.打开Origin所在的`User Files Folder`，然后找到`OriginC`这个文件夹（如果没有，新建一个），在这个文件夹下新建一个文件夹，命名为`Origin_Call_GSL`，这个名字可以是任命合法的名字，不一定是这个名字。

2.把两个`dll`解压到`Origin_Call_GSL`文件夹里。

3.在Origin（需要32位的Origin）的Code Builder新建一个头文件，名字可以为`Origin_Call_GSL.h`，保存到`Origin_Call_GSL`文件夹。在这个头文件里，主要的包括两部分：载入`dll`和GSL函数的原型说明。
```
/*
pragma指令是告诉Origin C到哪去链接函数体，
header的意思就是在与头文件相同的文件夹里
*/
#pragma dll(libgsl, header) 
```

```
/*
声明GSL函数原型
可以直接从GSL头文件复制过来
此函数文档： 
http://www.gnu.org/software/gsl/doc/html/lls.html#linear-regression-with-a-constant-term
*/
GSL_EXPORT int gsl_fit_linear(const double * x, const size_t xstride, const double * y, const size_t ystride, size_t n, double * c0, double * c1, double * cov00, double * cov01, double * cov11, double * sumsq);
```

4.然后新建一个`c`文件，名为`Origin_Call_GSL.c`，保存在与头文件一个文件夹上，并加入Code Builder的工作空间中。

第一步当然是要把头文件引进来。

```
#include "Origin_Call_GSL.h"  // 引入头文件
```

第二步就可以新建一个Origin C函数来调用头文件里声明的GSL函数了。

```
void Origin_Call_GSL() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	size_t n;
	double *x, *y;
	size_t xstride, ystride;
	double c0, c1, cov00, cov01, cov11, sumsq;
	
	// 准备数据
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
	
	// 调用GSL函数
	gsl_fit_linear(x, 1, y, 1, n, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
	printf("\n回归常数项(截距) c0 = %6.4f\n", c0);
	printf("回归系数(斜率) c1 = %6.4f\n", c1);
	printf("方差与协方差矩阵 cov00 = %6.4f  cov01 = %6.4f  cov11 = %6.4f\n", cov00, cov01, cov11);
	printf("残差平方和 sumsq = %6.4f\n", sumsq);
	
	free(x);
	free(y);
}
```

**注意事项：**

1.要同时支持32位和64位，除了要同时提供32位和64位的`dll`外，在加载`dll`的代码也有讲究。

```
#ifdef _OWIN64  // 这个宏是判断当前Origin是不是64位的
#pragma dll(libgsl_64, header)  // 假如64位的dll叫libgsl_64
#else
#pragma dll(libgsl, header)
#endif  // _OWIN64
```

2.如果要调用的GSL函数返回值的类型是结构体的，对不起，Origin C不支持使用这类函数。

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
