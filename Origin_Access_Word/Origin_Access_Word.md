Origin C要往Word里写内容，一个简单的方法是通过Word里的书签来定位，然后直接获取这个书签位置来写入内容。

用到的Word文档是Origin自带的一个模板，位于Origin的安装目录下`Samples\Batch Processing\Sensor Analysis Report.dotx`。

这个模板包含有以下的书签：`FileName, FileDate, FittedGraph, ResidualPlot, InterceptValue, InterceptError, SlopeValue, SlopeError, RSquare`。

在Word里要把书签显示出来，可以通过`文件>选项>高级`，然后找到`显示文档内容`那一栏，把`显示书签`打上勾。

主要步骤：

1.创建Word应用
```
wApp = CreateObject("Word.Application");  // 创建Word应用
```

2.通过模板打开Word文档
```
wDoc = wApp.Documents.Open(strWordTemplate, false, false);  // 通过模板打开Word文档
```

3.获得书签对象
```
if (wDoc.Bookmarks.Exists(vsTextBookmarks[ii]) == false) {  // 先检测书签存不存在
    continue;
}
Object wBookmark = wDoc.Bookmarks(vsTextBookmarks[ii]);  // 获得书签
```

4.通过书签对象来填内容
```
wBookmark.Range.Text = vsTexts[ii];  // 把文本放上去

wBookmark.Range.Text = ftoa(vNumbers[ii]);  // 先把数字转为字符串，本质上是把文本放上去

string strLT = "Clipboard -CE " + vsGraphNames[ii] + ";";  // 复制图的LabTalk语句
LT_execute(strLT);  // 执行LabTalk语句
wBookmark.Range.Paste();  // 把图粘贴到书签的位置
```

5.保存文档，这里同时保存为Word和PDF
```
wDoc.SaveAs(strSavedWordFile, 16);  // wdFormatDocumentDefault == 16，默认格式，Word 2010为docx	
wDoc.SaveAs(strSavedPDFFile, 17);  // wdFormatPDF == 17, PDF格式
```

6.关闭并退出Word应用
```
wDoc.Close(0);  // wdDoNotSaveChanges == 0，不保存关闭
wApp.Quit();  // 退出Word应用
```

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
