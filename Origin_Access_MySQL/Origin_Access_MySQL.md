Origin可以用界面的方式来导入数据库的数据到工作表，对应的界面对话框为`SQL Editor`和`Query Builder`。`SQL Editor`是Origin推荐使用的，`Query Builder`是比较老的一个工具。

Origin也可以通过Origin C编程的方式来导入数据库的数据，也可以把工作表的数据导出到数据库。这里演示用Origin C来导入导出MySQL的数据。

以下安装都选择64位的。

1.安装MySQL，这里用到的版本是`8.0.11`

MySQL官网下载一个社区的免费版，[https://dev.mysql.com/downloads/mysql/](https://dev.mysql.com/downloads/mysql/)。自行安装。记住用户名和密码，这里直接用`root`帐户也没有问题。

2.为了使用界面操作，安装Workbench

也是到MySQL官网下载，[https://dev.mysql.com/downloads/workbench/](https://dev.mysql.com/downloads/workbench/)。

打开Workbench，看能不能登陆。服务器名为`localhost`，端口为`3306`，帐户为`root`，密码设置了什么就填什么。登陆后看有没有`world`这个数据库，这是MySQL自带的。如果没有登陆上，或者登陆上没有这个数据库，那就。。。拜拜！

3.安装ODBC

还是MySQL官网，[https://dev.mysql.com/downloads/connector/odbc/](https://dev.mysql.com/downloads/connector/odbc/)。

4.安装完ODBC，查看一下驱动

打开`控制面版`，找到`系统和安全`，然后打开`管理工具`，双击`ODBC数据源（64 位）`打开，然后在`驱动`那个标签看有没有类似于`MySQL ODBC 8.0 Unicode Driver`这样的驱动，有就记下来。

连接字符串的编写是这样的：
```
Driver={MySQL ODBC 8.0 Unicode Driver};  大括号里就是驱动的名称
Server=localhost;  MySQL安装的机器名，本地就localhohst
Port=3306;  端口
Database=world;  数据库名
UID=root;  帐户名
PWD=xxx  密码
```

5.数据库准备好了就可以开始Origin的操作

打开64位的Origin，再打开Code Builder，新加一个名为`Origin_Access_MySQL.c`文件，并加入到工作区间。

首先是导入数据库的代码：
```
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
```
运行这个函数可以把`world`这个数据库下的`city`表导入到Origin的工作表中。

6.然后来试一下把刚才导入的工作表的数据添加到数据库中。

回到Workbench，新建一个数据库名为`test`，然后运行以下SQL语句来生成一个结构跟`city`这个表差不多的一个表。
```
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
```

生成后，回到Code Builder，开始把工作表的数据导出到数据库。

```
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
```
运行这个函数时，刚才导入数据生成的工作表要处于当前工作表状态。

至此，实现了Origin C对MySQL数据库的访问，包括查询数据然后导入Origin的工作表、把Origin工作表导出插入到数据库。
