#include <Origin.h>

/*
演示Origin C导入Excel中的数据到Origin的工作表。

输出为：
Sheet1
使用了259行，11列

同时在Origin新建了一个工作表，并且Excel的指定区域数据导入到这个工作表
*/
void Origin_Access_Excel() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	Object excelApp, excelWbk;
	
	// Excel文件
	string strExcelFile = GetOriginPath() + "Samples\\COM Server and Client\\MS Office\\Excel\\FFTFiltering.xls";
	try {
		excelApp = CreateObject("excel.application");  // 创建Excel应用
		
		/*
		https://docs.microsoft.com/en-us/previous-versions/office/developer/office-2007/bb179167(v=office.12)
		expression.Open(FileName, UpdateLinks, ReadOnly, Format, Password, WriteResPassword, IgnoreReadOnlyRecommended, 
						Origin, Delimiter, Editable, Notify, Converter, AddToMru, Local, CorruptLoad)
		Origin不支持图表的导入，第二个参数设置为0，第二个参数为true，以只读方式打开
		*/
		excelWbk = excelApp.Workbooks.Open(strExcelFile, 0, true);  // 打开Excel文件
	} catch (int err) {
		if (!excelApp) {
			printf("打开Excel应用失败！");
			return;
		}
		printf("打开文件（%s）失败！", strExcelFile);
		excelApp.Quit();  // 退出Excel应用
		return;
	}
	excelApp.Visible = true;  // 显示打开的文档，否则会在后台运行
	
	// 打印Excel工作表的名字
	for (int i = 1; i <= excelWbk.Worksheets.Count; i++) {
		string wksName = excelWbk.Worksheets.Item(i).Name;
		printf("%s\n", wksName);
	}
	
	Object excelWks = excelWbk.Worksheets.Item(1);  // Excel中的第一个工作表
	if (!excelWks) {
		printf("获取工作表失败！");
		excelWbk.Close(false);  // 关闭文件，以防Excel问要不要保存
		excelApp.Quit();  // 退出Excel应用
		return;
	}
	
	// 看一下工作表中有多少行多少列已经使用了
	int nUsedRows = excelWks.UsedRange.Rows.Count;
	int nUsedCols = excelWks.UsedRange.Columns.Count;
	printf("使用了%d行，%d列\n", nUsedRows, nUsedCols);
	
	// 导入AB两列数据，列标签在19行，数据从20行开始，到259行结束
	string strLabelA = excelWks.Cells(19, 1);  // 拿到第19行第1列所在格的数据
	string strLabelB = excelWks.Cells(19, 2);  // 拿到第19行第2列所在格的数据
	Object excelRange;
	excelRange = excelWks.Range("A20:B259");
	if (!excelRange) {
		printf("获取工作表指定范围区域失败！");
		excelWbk.Close(false);  // 关闭文件，以防Excel问要不要保存
		excelApp.Quit();  // 退出Excel应用
		return;
	}
	// 取出指定范围区域的数据
	_VARIANT varValues;
	varValues = excelRange.Value;
	
	// 把取出的数据放入Origin的工作表
	Worksheet wks;
	wks.Create("Origin");  // 新建工作表
	wks.SetSize(-1, 2);  // 设置工作表为2列
	wks.PutData(varValues);  // 把数据放入Origin工作表
	wks.Columns(0).SetLongName(strLabelA);  // 设置列的LongName
	wks.Columns(1).SetLongName(strLabelB);  // 设置列的LongName
	
	excelWbk.Close(false);  // 关闭文件，以防Excel问要不要保存
	excelApp.Quit();  // 退出Excel应用
}
