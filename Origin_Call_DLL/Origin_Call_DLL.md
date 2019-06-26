1.建立头文件声明要调用DLL的函数

2.在头文件中加入载入DLL的代码

载入DLL使用到`pragma`这个指令。例如DLL的名称为`MyDLL.dll`，则可以根据它所在位置有几种方法载入：
```
#pragma dll(MyDLL)  // dll位于Origin的安装目录下
#pragma dll(C:\xxx\xxx\MyDLL)  // 具体dll的位置
#pragma dll(MyDLL, header)  // dll所在的位置与头文件在同一目录
#pragma dll(MyDLL, system)  // dll位于Windows的系统文件夹
```

3.DLL版本控制

Origin包括有32位和64位的版本，调用时DLL的版本也要一致。在载入DLL的指令中，可以通过宏来控制版本的选择。
```
#ifdef _OWIN64  // 这个宏是判断当前Origin是不是64位的
#pragma dll(MyDLL_64, header)  // 假如64位的dll叫libgsl_64
#else
#pragma dll(MyDLL_32, header)
#endif  // _OWIN64
```

**演示例子：**
1.建立一个项目来生成DLL

在`c`盘中新建一个文件夹，名为`Create_DLL`，然后在此文件夹中新建一个`c`文件，名为`Origin_Call_DLL.c`。然后在这个文件中加入代码：
```
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
```
2.编译生成DLL

参考[Windows下编译64位GSL](https://www.cnblogs.com/Ooman/p/11080694.html)，去到`c:\msys64\`，双击运行`mingw64.exe`。在打开的窗口运行以下两行：
```
cd /c/Create_DLL
gcc -shared Origin_Call_DLL.c -o Origin_Call_DLL_64.dll
```
然后在`c:\Create_DLL\`下会生成`Origin_Call_DLL_64.dll`。如需要32位的DLL，可以用32位的编译器来生成。

3.在Origin里使用这个DLL

打开Origin所在的`User Files Folder`，然后找到OriginC这个文件夹（如果没有，新建一个），在这个文件夹下新建一个文件夹，命名为`Origin_Call_DLL`，这个名字可以是任命合法的名字，不一定是这个名字。把生成的DLL复制到这个文件夹下。

在Origin（这里生成是64位的DLL，需要64位的Origin）的Code Builder新建一个头文件，名字可以为`Origin_Call_DLL.h`，保存到`Origin_Call_DLL`文件夹。在这个头文件里，主要的包括两部分：载入DLL和函数的原型说明。
```
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
```
再新建一个`c`文件，`Origin_Call_DLL.c`，保存到`Origin_Call_DLL`文件夹，并加入Code Builder的工作空间中。然后在这个`c`文件中就可以通过引入头文件，从而去调用DLL里的函数。
```
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
```

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
