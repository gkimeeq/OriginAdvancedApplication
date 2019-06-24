/*
pragma指令是告诉Origin C到哪去链接函数体，
header的意思就是在与头文件相同的文件夹里
*/
#pragma dll(libgsl, header) 

#define GSL_EXPORT  // 对于Origin C来说，这一句不是必须的，保留空值即可

/*
声明GSL函数原型
可以直接从GSL头文件复制过来
此函数文档： 
http://www.gnu.org/software/gsl/doc/html/lls.html#linear-regression-with-a-constant-term
*/
GSL_EXPORT int gsl_fit_linear(const double * x, const size_t xstride, const double * y, const size_t ystride, size_t n, double * c0, double * c1, double * cov00, double * cov01, double * cov11, double * sumsq);
