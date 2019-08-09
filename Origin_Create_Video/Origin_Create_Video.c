#include <Origin.h>
#include <import_image.h>  // 这个头文件包含了导入图片和获取图片信息的函数
#include <..\OriginLab\VideoWriter.h>  // 这个头文件包含了VideoWriter类，用于创建视频

/*
演示Origin C导入图片，并把导入的图片做成视频

输出为：
在User Files Folder创建一个视频文件
*/
void Origin_Create_Video() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	// 在Origin安装目录下的子目录Samples\Image Processing and Analysis\下，有8张myocyte的tif图片
	string strImageFileBase = GetOriginPath() + "Samples\\Image Processing and Analysis\\myocyte";
	vector<string> vsImageFiles(8);
	for (int i = 1; i <= 8; i++) {  // 构建好8张图片的完整路径
		vsImageFiles[i-1] = strImageFileBase + i + ".tif";
	}
	
	MatrixPage mp;
	mp.Create("Origin");  // 创建矩阵窗口
	if (!mp) {  // 创建矩阵窗口失败则返回
		printf("创建视频失败！");
		return;
	}
	MatrixLayer ml = mp.Layers(0);  // 获取矩阵窗口的第一个矩阵层
	if (!ml) {  // 矩阵层不存在，返回
		printf("创建视频失败！");
		return;
	}
	MatrixObject mo = ml.MatrixObjects(0);  // 获取矩阵层的第一个矩阵对象，用于存放导入的图片
	if (!mo) {  // 矩阵对象不存在，返回
		printf("创建视频失败！");
		return;
	}
	GraphPage gp;
	gp.Create("image");  // 创建画图窗口，使用image这个模板，用于画图片
	if (!gp) {  // 创建画图窗口失败则返回
		printf("创建视频失败！");
		return;
	}
	// 设置画图窗口的尺寸
	int width = 1024;  // 宽
	int height = 1024;   // 高
	Tree trFormat;
	trFormat = gp.GetFormat(FPB_ALL, FOB_ALL, true, true);  // 获得画图窗口的属性树
	trFormat.Root.Dimension.Units.nVal = 3;  // 单位为像素
	trFormat.Root.Dimension.Width.dVal = width;  // 宽
	trFormat.Root.Dimension.Height.dVal = height;  // 高
	trFormat.Root.Dimension.KeepRatio.nVal = 0;  // 不保持宽高比
	gp.UpdateThemeIDs(trFormat.Root);  // 允许没有主题ID的属性可以应用
	gp.ApplyFormat(trFormat, true, true);  // 设置新的属性
	GraphLayer gl = gp.Layers(0);  // 获取画图窗口的第一个图层
	if (!gl) {  // 图层不存在，返回
		printf("创建视频失败！");
		return;
	}
	// 由于创建视频时，一个画图窗口作为一帧，为了让图片充满整个画图窗口，把图层的尺寸改为与画图窗口一致
	Tree tr;
	tr = gl.GetFormat(FPB_ALL, FOB_ALL, true, true);  // 获得图层的属性树
	// tr.Root.Dimension.Units.nVal = 0;  // 默认即为0，即以画图窗口的百分比来算图层的大小
	tr.Root.Dimension.Left.dVal = 0;  // 图层位于画窗口左边的距离设置为0
	tr.Root.Dimension.Top.dVal = 0;  // 图层位于画窗口上面的距离设置为0
	tr.Root.Dimension.Width.dVal = 100;  // 图层的宽度为画图窗口的100%宽度
	tr.Root.Dimension.Height.dVal = 100;  // 图层的高度为画图窗口的100%高度
	gl.UpdateThemeIDs(tr.Root);  // 允许没有主题ID的属性可以应用
	gl.ApplyFormat(tr, true, true);  // 设置新的属性
	
	gl.AddPlot(mo, IDM_PLOT_MATRIX_IMAGE);  // 把矩阵对象以图片的形式画到图层上
	
	/*
	定认视频编码，不要期望所有的电脑都可以使用所有的编码
	使用宏CV_FOURCC来定义，需要4个参数
	Origin也预定义了一些常用的编码，在VideoWriter.h头文件中
	#define VW_CODEC_MPEG1		CV_FOURCC('P', 'I', 'M', '1') // MPEG-1 codec
	#define VW_CODEC_MJPEG		CV_FOURCC('M', 'J', 'P', 'G') // motion-jpeg codec (does not work well)
	#define VW_CODEC_MPEG42		CV_FOURCC('M', 'P', '4', '2') // MPEG-4.2 codec
	#define VW_CODEC_MPEG43     CV_FOURCC('D', 'I', 'V', '3') // MPEG-4.3 codec
	#define VW_CODEC_MPEG4		CV_FOURCC('D', 'I', 'V', 'X') // MPEG-4 codec
	#define VW_CODEC_H263		CV_FOURCC('U', '2', '6', '3') // H263 codec
	#define VW_CODEC_H263I		CV_FOURCC('I', '2', '6', '3') // H263I codec
	#define VW_CODEC_FLV1		CV_FOURCC('F', 'L', 'V', '1') // FLV1 codec
	#define VW_CODEC_INVALID	CV_FOURCC('!', '!', '!', '!') // 不合法的编码
	
	CV_FOURCC的四个参数的一些可用组合：
	0, 0, 0, 0  No compress,raw.  
	'F', 'M', 'P', '4'  MPEG4, mencoder的默认MPEG4格式
	'D', 'I', 'V', 'X'  DivX, 4.0或更高版本
	'M', 'P', '4', '2'  MSMPEG4v2, 一个MPEG-4变种
	'M', 'P', '4', '3'  MSMPEG4v3, 一个MPEG-4变种
	'W', 'M', 'V', '1'  WMV1  
	'W', 'M', 'V', '2'  WMV2  
	'M', 'J', 'P', 'G'  MJPEG, Motion JPEG视频
	'M', 'J', 'L', 'S'  JPEG-LS  
	'H', 'F', 'Y', 'U'  HUFFYUV, Huffman编码，用于YUV和RGB格式
	'F', 'F', 'V', 'H'  FFVHUFF  
	'M', 'S', 'V', 'C'  MSVC, Windows视频自带的编码
	'c', 'v', 'i', 'd'  Cinepak  
	'I', 'V', '3', '1'  Indeo3  
	'I', 'V', '5', '0'  Indeo5  

	// int codec = CV_FOURCC(0, 0, 0, 0);  // 不压缩，大文件
	// int codec = CV_FOURCC(1, 0, 0, 0);  // Microsoft Run Length Encoded 
	*/
	int codec = CV_FOURCC('D', 'I', 'V', 'X'); // 这里用MPEG-4编码
	VideoWriter videoWriter;  // 用于创建视频的类
	string strVideoPath = GetOriginPath(ORIGIN_PATH_USER) + "myocyte.avi";  // 放于User Files Folder路径下
	int fps = 10;  // 每秒帧数
	int err = videoWriter.Create(strVideoPath, codec, fps, width, height);
	if (err != 0) {
		printf("创建视频失败！");
		return;
	}
	
	// 导入图片作为视频的每一帧
	for (i = 0; i < vsImageFiles.GetSize(); i++) {  // 这里i不用再声明可以直接用，如果再声明会编译报错，应该是Origin C的Bug吧
		if (!vsImageFiles[i].IsFile()) {  // 如果图片路径不是文件，跳过
			continue;
		}
		int nWidth, nHeight, nBPP;  // 图片的宽、高、像素位数（bit per pixel）
		if (!oimg_image_info(vsImageFiles[i], &nWidth, &nHeight, &nBPP)) {  // 获取图片信息，如果失败，跳过
			continue;
		}
		if (!mo.SetSize(nHeight, nWidth, 0)) {  // 设置矩阵对象的大小，跟图片大小一样，如果失败，跳过
			continue;
		}
		if (!mo.SetInternalData(FSI_BYTE, false, false)) {  // 设置矩阵对象内在数据的类型，BYTE，8位
			continue;
		}
		Matrix<BYTE> &m = mo.GetDataObject();  // 获得矩阵对象关联的内在数据，以引用的方式，m的修改会影响矩阵对象的内在数据
		oimg_load_image(vsImageFiles[i], &m, 8, nHeight, nWidth);  // 导入图片到矩阵对象，则改变矩阵对象的数据，图也会更新
		
		gl.Rescale();  // 重新调节图层的标尺
		LT_execute("sec -pw " + gp.GetName() + ";");  // 等待画图窗口更新完成
		
		for (int j = 0; j < fps; j++) {  // 每张图片显示1秒
			err = videoWriter.WriteFrame(gp);  // 写入一帧
			if (err != 0) {  // 写入帧失败
				printf("创建视频失败！");
				break;
			}
		}
		if (err != 0) {
			break;
		}
	}
	videoWriter.Release();  // 释放资源
	if (err == 0) {
		printf("创建视频成功！视频保存于：%s", strVideoPath);
	}
	
	mp.Destroy();  // 删除矩阵窗口
	gp.Destroy();  // 删除画图窗口
}
