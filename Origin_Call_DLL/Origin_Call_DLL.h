#ifdef _OWIN64  // 这个宏是判断当前Origin是不是64位的
#pragma dll(Origin_Call_DLL_64, header) 
#else
#pragma dll(Origin_Call_DLL_32, header)
#endif  // _OWIN64

// 返回圆周率
double GetPi();

// 圆
typedef struct _tagCircle {
	double cx;
	double cy;
	double radius;
} Circle;

// 通过指针返回结构体
void GetCircle(Circle *pCircle);