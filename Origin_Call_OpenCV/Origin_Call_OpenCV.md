Origin对OpenCV进行了包装，因此在Origin C中可以调用OpenCV的相关函数。下面就用OpenCV官网的一个例子来演示怎么在Origin C里调用OpenCV。

1.包含头文件

```
#include <../OriginLab/opencv.h>
```

这个头文件其实就是又包含了其它的一些头文件，包括OpenCV的类型相关的、核心的、图像处理的、Origin自己实现的，等等。因此包含这个头文件进来基本可以没压力的调用相关的OpenCV函数了。

2.OpenCV官网文档的例子

[https://docs.opencv.org/3.0.0/d4/d70/tutorial_hough_circle.html](https://docs.opencv.org/3.0.0/d4/d70/tutorial_hough_circle.html)

在Origin C里的实现如下：

 ```
void Origin_Call_OpenCV() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	string strImageFile = GetFilePath(__FILE__) + "board.jpg";  // 图片路径
	CvMat *pImage = cvLoadImageM(strImageFile, CV_LOAD_IMAGE_COLOR);  // 导入图片
	if (pImage == NULL) {  // 导入失败
		printf("打开图片失败！");
		return;
	}
	int rows = pImage->rows;  // 图片高度
	int cols = pImage->cols;  // 图片宽度
	printf("图片的宽度为%d，高度为%d\n", cols, rows);
	ShowImage(pImage);  // 在Origin里显示图片
	
	CvMat grayImage;
	matrix<byte> matGray(rows, cols);  // 灰度图的数据
	grayImage = cvMat(rows, cols, CV_8UC1, &matGray[0][0]);
	cvCvtColor(pImage, &grayImage, CV_BGR2GRAY);  // 转为灰度图
	
	ocvMat ocvmat;  // 这个类是Origin定义的
	ocvmat.SetFromMat(&grayImage, true);  // 把OpenCV的数据放入Origin定义那个类
	ocvmat.MedianBlur(5);  // 中值滤波
	CvMat *pGrayImage = ocvmat.CloneCvMat();     // 克隆Origin定义那个类的数据到OpenCV的数据类，要用cvReleaseMat释放内存
	ShowImage(pGrayImage);  // 在Origin里显示图片
	
	CvMemStorage *pMemStorage = cvCreateMemStorage(0);  // 创建内存
	// 霍夫变换算法的参数
	double dp = 1;
	double minDist = 10;
	double param1 = 100;
	double param2 = 30;
	int minRadius = 1;
	int maxRadius = 30;
	// 霍夫变换找圆
	CvSeq *pSeq = cvHoughCircles(pGrayImage, pMemStorage, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
	int total = pSeq->total;  // 一共找到多少圆
	for (int i = 0; i < total; i++) {
		float *p = (float *)cvGetSeqElem(pSeq, i);  // p[0], p[1]为圆心，p[2]为半径
		CvPoint pt;
		pt.x = cvRound(p[0]);
		pt.y = cvRound(p[1]);
		CvScalar scalar;  // e用来设置颜色
		scalar = cvScalar(0, 0, 255, 0xff);  // BGR=(0, 0, 255)，圆的颜色
		int thickness = 3;  // 线的宽度
		int lineType = 16;  // LINE_AA，抗锯齿
		ocvCircle(pImage, &pt, cvRound(p[2]), &scalar, thickness, lineType);  // 画找到的圆
		scalar = cvScalar(0, 255, 0, 0xff);  // BGR=(0, 0, 255)，圆心的颜色
		ocvCircle(pImage, &pt, 2, &scalar, thickness, lineType);  // 画圆心
	}
	ShowImage(pImage);  // 在Origin里显示图片
	string strSavedImage = GetFilePath(__FILE__) + "result.png";  // 结果图路径
	cvSaveImage(strSavedImage, pImage);  // 把结果图保存到当前文件夹
	
	cvReleaseMat(&pGrayImage);  // 释放灰度图数据
	cvReleaseMemStorage(&pMemStorage);  // 释放内存
	cvReleaseMat(&pImage);  // 释放原图数据
}
```

在Origin里显示的结果图为：

![Origin里显示的结果图](https://img2018.cnblogs.com/blog/668287/201907/668287-20190719133150930-1123990336.png)

导出的结果图为：

![导出的结果图](https://img2018.cnblogs.com/blog/668287/201907/668287-20190719133230633-1711156549.png)

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
