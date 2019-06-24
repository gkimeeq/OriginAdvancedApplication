NAG（Numerical Algorithms Group, [www.nag.com](http://www.nag.com)）库是一个无与伦比的算法库，它提供的算法可靠、轻便、严谨，覆盖了数学与统计方方面面。最大的缺点就是：它是一个收费的算法库。

Origin从OriginPro8开始就包含了NAG库的所有函数，通过Origin C可以轻易调用所有这些NAG函数。

Origin C调用NAG库函数的主要说明：

1.头文件
```
#include <OC_nag.h>  // 这个头文件包括所有通用的NAG头文件
```
2.`NagError`结构体，Origin C里的定义如下：
```
typedef struct {
    int code;    // Out: Error Code
    BOOL    print;   // In: print? yes/no, not used in Origin C 
    char    message[NAG_ERROR_BUF_LEN];      // InOut: Error message
    UINT    handler; //Error handling function, not used from OriginC
    int errnum;  // May hold useful value for some errors
} NagError;
```
所有的NAG库的函数都接受`NagError`结构体的指针作为其中的一个参数。通过这个结构体可以测试对应调用的函数是否成功。当调用完函数后，可以通过测试`code`的值是否为`NE_NOERROR`（`#define NE_NOERROR 0`）来判断是否成功。

下面演示如何通过调用NAG库的函数来做一个简单的线性回归。用到的NAG库函数为：

```
void nag_simple_linear_regression (Nag_SumSquare mean, 
       Integer n, const double x[], const double y[], 
       const double wt[], double *a, double *b, 
       double *a_serr, double *b_serr, double *rsq, 
       double *rss, double *df, NagError *fail)
```
调用的时候，函数名字除了用`nag_simple_linear_regression`，也可以用`g02cac`，后者有点像是代号一样。

**参数说明：**

1.`mean`: 类型为`Nag_SumSquare`，Origin C里这样定义：
```
#define Nag_SumSquare_start 1362
typedef enum {Nag_AboutMean=Nag_SumSquare_start, Nag_AboutZero} Nag_SumSquare;
```
这个参数指定是否把直线模型`y=a+bx`中的`a`常数项包括入回归当中。如果`mean`的值为`Nag_AboutMean`，则回归常项包括进来，否则为`Nag_AboutZero`就不包括常数项。

2.`n`：观察值的个数。如果`mean == Nag_AboutMean`，`n`要大于等于`2`，如果`mean == Nag_AboutZero`，`n`要大于等于`1`。

3.`x[]`：自变量。

4.`y[]`：因变量。

5.`wt[]`：权重。如果没有权重，则为`NULL`。

6.`a`：输出值。如果`mean == Nag_AboutMean`，输出值为回归常数项，否则为`0`。

7.`b`：输出值。线性模型的系数。

8.`a_serr`：输出值。回归常数项的标准差。

9.`b_serr`：输出值。系数的标准差。

10.`rsq`：输出值。判定系数。

11.`rss`：输出值。残差平方和。

12.`df`：输出值。自由度。

13.`fail`: `NagError`结构体，可以用来判断回归是否成功。

函数代码如下：
```
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
    nag_simple_linear_regression(mean, n, x, y, wt, 
    			     &a, &b, &a_serr, &b_serr, 
                                 &rsq, &rss, &df, &fail); 
    // 或者这样
    //g02cac(mean, n, x, y, wt, &a, &b, &a_serr, 
    //&b_serr, &rsq, &rss, &df, &fail); 
    
    if (fail.code != NE_NOERROR) {  // 回归失败
        printf("错误来自于nag_simple_linear_regression(g02cac)。\n%s\n", 
        fail.message);
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
```
源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
