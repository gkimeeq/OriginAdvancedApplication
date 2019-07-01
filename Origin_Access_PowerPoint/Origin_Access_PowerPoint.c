#include <Origin.h>
//#include <..\originlab\PowerPoint.h>  // 引入头文件，用Origin内置类

/*
演示打开一个PowerPoint文件，并把两张图分别复制粘贴到两张幻灯片中。
*/
void Origin_Access_PowerPoint_New() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	// 1.创建一个PowerPoint应用对象
	Object ppt;
	// 新建一个PowerPoint应用
	try {
		ppt = CreateObject("powerpoint.application");
	} catch (int err) {
		printf("新建PowerPoint出错！");
	}
	if (ppt == NULL) {
		printf("新建PowerPoint出错！");
		return;
	}
	ppt.Visible = true;  // 显示出来，否则会在后台运行
	
	// 2.创建一个演示对象，即一个PowerPoint文件
	// 打开一个PowerPoint文件
	Object presentation;
	// PowerPoint文件的全路径
	string strTemplate = GetOriginPath(ORIGIN_PATH_SYSTEM) + "Samples\\Import and Export\\Column_Plots_in_Sample_OPJ.pptx";
	presentation = ppt.Presentations.Open(strTemplate);
	if (presentation == NULL) {
		printf("打开文件失败！");
		return;
	}
	
	// 3.获取幻灯片
	int numSlide = presentation.Slides.Count;  // 一共有多少张幻灯片
	Object slide;
	if (numSlide < 1) {  // 如果一张幻灯片都还没有，就新加一张
		slide = presentation.Slides.Add(1, 12);  // ppLayoutBlank == 12, 新增一张空幻灯片
	} else {
		slide = presentation.Slides(numSlide-1);  // 倒数第二张幻灯片
	}
	// 插入一张幻灯片，用前一张作为模板
	Object slide2;
	slide2 = slide.Duplicate();
	
	// 4.复制Origin的图，然后粘贴到幻灯片
	string strGraphPageName = "Graph1";  // 要保证有Graph1这个图
	GraphPage gp(strGraphPageName);
	if (!gp) {
		printf("粘贴失败！");
		return;
	}
	gp.LT_execute("Clipboard %H");  // 复制Origin的图到剪贴板
	Object shapes;
	shapes = slide.Shapes;
	if (shapes == NULL) {
		printf("粘贴失败！");
		return;
	}
	shapes.PasteSpecial(3);  // ppPasteMetafilePicture == 3，粘贴到PowerPoint
	
	string strGraphPageName2 = "Graph2";  // 要保证有Graph2这个图
	GraphPage gp2(strGraphPageName2);
	if (!gp2) {
		printf("粘贴失败！");
		return;
	}
	gp2.LT_execute("Clipboard %H");  // 复制Origin的图到剪贴板
	Object shapes2;
	shapes2 = slide2.Shapes;
	if (shapes2 == NULL) {
		printf("粘贴失败！");
		return;
	}
	shapes2.PasteSpecial(3);  // ppPasteMetafilePicture == 3，粘贴到PowerPoint
	
	// 5.保存PowerPoint文件
	presentation.SaveAs("D:\\test.pptx");
	
	// 6.退出PowerPoint
	ppt.Quit();
}

/*
演示把两张图分别复制粘贴到已经打开的PowerPoint文件的两张幻灯片中。

先打开Origin安装目录下的Samples\Import and Export\Column_Plots_in_Sample_OPJ.pptx文件
*/
void Origin_Access_PowerPoint_Active() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	// 1.获取一个PowerPoint应用对象
	Object ppt;
	// 获取已经打开的PowerPoint应用
	try {
		ppt = GetActiveObject("powerpoint.application", false);
	} catch(int err) {
		printf("获取打开的PowerPoint出错！");
	}
	if (ppt == NULL) {
		printf("获取打开的PowerPoint出错！");
		return;
	}
	
	// 2.获得一个演示对象，即一个PowerPoint文件
	Object presentation;
	int numPres = ppt.Presentations.Count;  // 当前PowerPoint应用打开的所有演示文件
	if (numPres < 1) {  // 如果没有文件打开，新加一个
		presentation = ppt.Presentations.Add(1);  // 新加一个文件
	} else {
		presentation = ppt.Presentations(numPres);  // 拿到最后一个打开的文件
	}
	if (presentation == NULL) {
		printf("打开演示文件失败！");
		return;
	}
	
	// 3.获取幻灯片
	int numSlide = presentation.Slides.Count;  // 一共有多少张幻灯片
	Object slide;
	if (numSlide < 1) {  // 如果一张幻灯片都还没有，就新加一张
		slide = presentation.Slides.Add(1, 12);  // ppLayoutBlank == 12, 新增一张空幻灯片
	} else {
		slide = presentation.Slides(numSlide-1);  // 倒数第二张幻灯片
	}
	// 插入一张幻灯片，用前一张作为模板
	Object slide2;
	slide2 = slide.Duplicate();
	
	// 4.复制Origin的图，然后粘贴到幻灯片
	string strGraphPageName = "Graph1";  // 要保证有Graph1这个图
	GraphPage gp(strGraphPageName);
	if (!gp) {
		printf("粘贴失败！");
		return;
	}
	gp.LT_execute("Clipboard %H");  // 复制Origin的图到剪贴板
	Object shapes;
	shapes = slide.Shapes;
	if (shapes == NULL) {
		printf("粘贴失败！");
		return;
	}
	shapes.PasteSpecial(3);  // ppPasteMetafilePicture == 3，粘贴到PowerPoint
	
	string strGraphPageName2 = "Graph2";  // 要保证有Graph2这个图
	GraphPage gp2(strGraphPageName2);
	if (!gp2) {
		printf("粘贴失败！");
		return;
	}
	gp2.LT_execute("Clipboard %H");  // 复制Origin的图到剪贴板
	Object shapes2;
	shapes2 = slide2.Shapes;
	if (shapes2 == NULL) {
		printf("粘贴失败！");
		return;
	}
	shapes2.PasteSpecial(3);  // ppPasteMetafilePicture == 3，粘贴到PowerPoint
}
