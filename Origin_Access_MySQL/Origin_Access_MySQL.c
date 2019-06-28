#include <Origin.h>

/*
演示Origin C导入MySQL的数据。
*/
void Origin_Access_MySQL_Query() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	// 数据库连接字符串
	string strConn = "Driver={MySQL ODBC 8.0 Unicode Driver};Server=localhost;Port=3306;Database=world;UID=root;PWD=xxxx";
	// 要执行的查询语句
	string strQuery = "Select ID, Name, CountryCode, District, Population from city limit 100;";
	
	Worksheet wks;
	wks.Create("Origin");  // 新建一个工作表
	if (!wks) {
		return;
	}
	
	Object mysql;
	mysql = CreateObject("ADODB.Connection");  // 连接对象
	if (!mysql) {
		return;
	}
	mysql.Open(strConn);  // 打开连接
	
	Object recordset;
	recordset = CreateObject("ADODB.Recordset");  // 记录集对象
	if (!recordset) {
		return;
	}
	recordset.CursorLocation = 3;  // 3 == adUseClient,使recordset为只读，可以加快打开速度
	recordset.Open(strQuery, mysql);  // 打开记录集
	
	// int rowBegin = 10;  // 从工作表的第几行开始填数据
	// int numRows = 100;  // 填多少条数据
	// 这个方法只返回布尔值，已经不推荐使用，推荐使用ReadRecordset
	// bool bb = wks.PutRecordset(recordset, rowBegin, numRows); 
	int err = wks.ReadRecordset(recordset);  // 返回0为成功
	if (err != 0) {
		printf("数据库读取失败！");
	}
}


/*

演示Origin C导出工作表到MySQL的数据。

在MySQL的test数据库下生成数据测试用的数据表的SQL语句：
CREATE TABLE `test`.`city` (
  `ID` INT(11) NOT NULL,
  `Name` CHAR(35) NOT NULL DEFAULT '',
  `CountryCode` CHAR(3) NOT NULL DEFAULT '',
  `District` CHAR(20) NOT NULL DEFAULT '',
  `Population` INT(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_bin;
*/
void Origin_Access_MySQL_Insert() {  // 定义一个Origin C函数，编译通过可以直接在Origin的Script Window直接运行
	// 数据库连接字符串
	string strConn = "Driver={MySQL ODBC 8.0 Unicode Driver};Server=localhost;Port=3306;Database=test;UID=root;PWD=xxxx";
	// 要执行的查询语句
	string strQuery = "Select ID, Name, CountryCode, District, Population from city;";
	
	Worksheet wks = Project.ActiveLayer();  // 当前工作表
	if (!wks) {
		return;
	}
	
	Object mysql;
	mysql = CreateObject("ADODB.Connection");  // 连接对象
	if (!mysql) {
		return;
	}
	mysql.Open(strConn);  // 打开连接
	
	Object recordset;
	recordset = CreateObject("ADODB.Recordset");  // 记录集对象
	if (!recordset) {
		return;
	}
	
	recordset.CursorLocation = 3;  // 3 == adUseClient, 使recordset为只读，可以加快打开速度
	// Recordset.Open([Source],[ActiveConnection],[CursorType],[LockType],[Options])
	// 1 == adOpenKeyset
	// 3 == adLockOptimistic, 当数据源正在更新时，系统并不会锁住其他用户的动作，其他用户可以对数据进行增、删、改的操作
	recordset.Open(strQuery, mysql, 1, 3);  // 打开记录集, 
	
	//int option = LAYWKSETRECORDSET_APPEND;  // 追加的形式插入
	int option = LAYWKSETRECORDSET_REPLACE;  // 覆盖的形式插入，如主键已存在，更新该条数据，主键不存在，则插入
	int rowBegin = 0;
	int numRows = 10;
	int colBegin = 0;
	int err = wks.WriteRecordset(recordset, option, rowBegin, numRows, colBegin);  // 返回0为成功
	if (err != 0) {
		printf("数据库读取失败！");
		return;
	}
	if (recordset.State == 1) {  // adStateOpen，记录集还是打开状态，则关闭
		recordset.Close();
	}
}
