#include <Origin.h>

/*
演示Origin C通过Word的书签来填内容，并保存成Word文档和PDF文档。

运行结果：
在D盘保存有两个文件：test.docx, test.pdf
*/
void Origin_Access_Word() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	Object wApp, wDoc;
	// Word模板
	string strWordTemplate = GetOriginPath() + "Samples\\Batch Processing\\Sensor Analysis Report.dotx";
	try {
		wApp = CreateObject("Word.Application");  // 创建Word应用
		wDoc = wApp.Documents.Open(strWordTemplate, false, false);  // 通过模板打开Word文档
	} catch (int err) {
		if (!wApp) {
			printf("打开Word应用失败！");
			return;
		}
		printf("打开文件（%s）失败！", strWordTemplate);
		wApp.Quit();  // 退出Word应用
		return;
	}
	
	vector<string> vsTextBookmarks = {"FileName", "FileDate"};  // 用来填文本的书签
	vector<string> vsNumberBookmarks = {"InterceptValue", "InterceptError", "SlopeValue", "SlopeError", "RSquare"};  // 用来填数字的书签
	vector<string> vsGraphBookmarks = {"FittedGraph", "ResidualPlot"};  // 用来放图的书签
	
	vector<string> vsTexts = {"Sensor01", "1/1/2018"};  // 要放的文本
	vector vNumbers = {2.298463, 0.002421, 1.456356, 0.034235, 0.998573};  // 要填的数字
	vector<string> vsGraphNames = {"Graph1", "Graph2"};  // 要放的图，要保证有这两个图存在
	
	try {
		int ii = 0;
		for (ii = 0; ii < vsTextBookmarks.GetSize(); ii++) {
			if (wDoc.Bookmarks.Exists(vsTextBookmarks[ii]) == false) {  // 先检测书签存不存在
				continue;
			}
			Object wBookmark = wDoc.Bookmarks(vsTextBookmarks[ii]);  // 获得书签
			wBookmark.Range.Text = vsTexts[ii];  // 把文本放上去
		}
		for (ii = 0; ii < vsNumberBookmarks.GetSize(); ii++) {
			if (wDoc.Bookmarks.Exists(vsNumberBookmarks[ii]) == false) {  // 先检测书签存不存在
				continue;
			}
			Object wBookmark = wDoc.Bookmarks(vsNumberBookmarks[ii]);  // 获得书签
			wBookmark.Range.Text = ftoa(vNumbers[ii]);  // 先把数字转为字符串，本质上是把文本放上去
		}
		for (ii = 0; ii < vsGraphBookmarks.GetSize(); ii++) {
			if (wDoc.Bookmarks.Exists(vsGraphBookmarks[ii]) == false) {  // 先检测书签存不存在
				continue;
			}
			GraphPage gp(vsGraphNames[ii]);  // 检测图存不存在
			if (!gp) {
				continue;
			}
			Object wBookmark = wDoc.Bookmarks(vsGraphBookmarks[ii]);  // 获得书签
			string strLT = "Clipboard -CE " + vsGraphNames[ii] + ";";  // 复制图的LabTalk语句
			LT_execute(strLT);  // 执行LabTalk语句
			wBookmark.Range.Paste();  // 把图粘贴到书签的位置
		}
	} catch (int err) {
		wDoc.Close(0);  // wdDoNotSaveChanges == 0，不保存关闭
		wApp.Quit();  // 退出Word应用
		return;
	}
	
	// 保存Word文档
	string strSavedWordFile = "d:\\test.docx";
	try {
		wDoc.SaveAs(strSavedWordFile, 16);  // wdFormatDocumentDefault == 16，默认格式，Word 2010为docx	
	} catch (int err) {
		wDoc.Close(0);  // wdDoNotSaveChanges == 0，不保存关闭
		wApp.Quit();  // 退出Word应用
		return;
	}
	
	// 保存为PDF文件
	string strSavedPDFFile = "d:\\test.pdf";
	try {
		wDoc.SaveAs(strSavedPDFFile, 17);  // wdFormatPDF == 17, PDF格式
	} catch (int err) {
		wDoc.Close(0);  // wdDoNotSaveChanges == 0，不保存关闭
		wApp.Quit();  // 退出Word应用
		return;
	}
	wApp.Quit();  // 退出Word应用
}
