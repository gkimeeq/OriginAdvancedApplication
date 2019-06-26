// 返回圆周率
double GetPi() {
	return 3.1415926;
}

// 圆
typedef struct _tagCircle {
	double cx;
	double cy;
	double radius;
} Circle;

// 通过指针返回结构体
void GetCircle(Circle *pCircle) {
	pCircle->cx = 5.0;
	pCircle->cy = 6.0;
	pCircle->radius = 10.0;
}
