#include <Origin.h>
#include "Origin_Call_DLL.h"  // 引入头文件

/*
演示Origin C调用DLL函数。

输出为：
圆周率Pi = 3.141593
圆心为（5.000000, 6.000000），半径为10.000000
*/
void Origin_Call_DLL() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	double dPi = GetPi();  // 调用DLL中的函数，并返回一个值
	printf("圆周率Pi = %.6f\n", dPi);
	Circle circle;
	GetCircle(&circle);  // 调用DLL中的函数，通过指针来获得值，Origin C不支持DLL中返回结构体
	printf("圆心为（%.6f, %.6f），半径为%.6f\n", circle.cx, circle.cy, circle.radius);
}