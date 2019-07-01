Origin C访问PowerPoint主要分6个步骤：

1.创建一个PowerPoint应用对象，或者获取当前打开的PowerPoint应用。

Origin C中通过`CreateObject`函数来创建。
```
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
```
而获取一个打开的PowerPoint应用对象则通过`GetActiveObject`来实现。
```
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
```
2.创建或获取演示对象，即打开的PowerPoint文件。可以打开一个PowerPoint文件，也可以获取打开的PowerPoint文件。

通过指定PowerPoint文件路径来打开。
```
// 打开一个PowerPoint文件
Object presentation;
// PowerPoint文件的全路径
string strTemplate = GetOriginPath(ORIGIN_PATH_SYSTEM) + "Samples\\Import and Export\\Column_Plots_in_Sample_OPJ.pptx";
presentation = ppt.Presentations.Open(strTemplate);
if (presentation == NULL) {
	printf("打开文件失败！");
	return;
}
```
也可以去拿已经打开的PowerPoint文件。
```
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
```
3.增加或获得PowerPoint文件中的幻灯片。
```
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
```
4.然后就可以用幻灯片对象来操作。

这里是把Origin里的图复制粘贴到PowerPoint。
```
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
```
5.保存PowerPoint文件。
```
presentation.SaveAs("D:\\test.pptx");
```
6.退出PowerPoint。
```
ppt.Quit();
```

Origin也提供了内置的类来操作PowerPoint，可以引入头文件`PowerPoint.h`，具体的类和方法可以直接打开这个头文件参考。
```
#include <..\originlab\PowerPoint.h>
```

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
