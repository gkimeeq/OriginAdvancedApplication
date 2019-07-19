#include <Origin.h>
#include <../OriginLab/opencv.h>  // 此头文件包含了OpenCV相关的函数

/*
演示Origin C使用OpenCV。
参考OpenCV官方的文档：https://docs.opencv.org/3.0.0/d4/d70/tutorial_hough_circle.html

输出为：
图片的宽度为640，高度为480

同时，在Origin里会生成3个图片窗口，分别显示原图、灰度图、结果图。结果图也保存在同一个文件夹下。
*/
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

// 这个函数是用来在Origin里显示图片的
void ShowImage(const CvMat *pImage) {
	ocvMat ocvmat;  // 这个类是Origin定义的
	ocvmat.SetFromMat(pImage, true);  // 把OpenCV的数据放入Origin定义那个类
	ImagePage imagePage;
	imagePage.Create("Origin");  // 创建Origin的图像窗口
	ImageLayer imageLayer = imagePage.Layers(0);  // 图像窗口的层
	imageLayer.SetMat(ocvmat, true);  // 把图像的数据放上层显示
}
