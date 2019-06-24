/*
pragmaָ���Ǹ���Origin C����ȥ���Ӻ����壬
header����˼��������ͷ�ļ���ͬ���ļ�����
*/
#pragma dll(libgsl, header) 

#define GSL_EXPORT  // ����Origin C��˵����һ�䲻�Ǳ���ģ�������ֵ����

/*
����GSL����ԭ��
����ֱ�Ӵ�GSLͷ�ļ����ƹ���
�˺����ĵ��� 
http://www.gnu.org/software/gsl/doc/html/lls.html#linear-regression-with-a-constant-term
*/
GSL_EXPORT int gsl_fit_linear(const double * x, const size_t xstride, const double * y, const size_t ystride, size_t n, double * c0, double * c1, double * cov00, double * cov01, double * cov11, double * sumsq);
