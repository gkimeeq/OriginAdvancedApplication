Origin C提供了`VideoWriter`类用于创建小视频。通过这个类，可以指定创建视频的编码格式，帧速，视频的宽度和高度等。然后可以把Origin中的画图窗口作为每一帧的图画来建立一个视频。

代码步骤：

1.引入头文件

```
#include <..\OriginLab\VideoWriter.h>
```

2.指定编码格式

```
int codec = CV_FOURCC('D', 'I', 'V', 'X');  // 这里用MPEG-4编码
```

3.创建视频文件

```
int width = 1024;  // 宽
int height = 1024;   // 高
VideoWriter videoWriter;  // 用于创建视频的类
string strVideoPath = GetOriginPath(ORIGIN_PATH_USER) + "myocyte.avi";  // 放于User Files Folder路径下
int fps = 10;  // 每秒帧数
int err = videoWriter.Create(strVideoPath, codec, fps, width, height);
```

4.写入帧

```
err = videoWriter.WriteFrame(gp);  // 写入一帧，gp为GraphPage窗口对象，即一个图作为一帧
```

5.释放VideoWriter

```
videoWriter.Release();  // 释放资源
```

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
