GSL （GNU Scientific Library, [http://www.gnu.org/software/gsl/](http://www.gnu.org/software/gsl/)）官方并没有提供编译好的Windows版本。首先要保证Windows是64位的。

1.下载msys2

从官网（[http://www.msys2.org/](http://www.msys2.org/)）下载`msys2-x86_64-20190524.exe`，当前的最新版本，64位的版本，这里是为了编译64位的，就不要下载32位的了。下载了就安装，直接安装到`c:\msys64`。

2.配置一波msys2

进入`c:\msys64`，找到`msys2.exe`，双击运行，或者从Windows的开始那里找，点`MSYS2 MinGW 64-bit`。然后运行：
```
pacman -Syu
```
有问`[Y/n]`之类的，敲`y`就可以了。更新完了，会有两个警告，然后就卡死了，直接窗口关了，当弹出一个类似于`Processes are running in session:`这样的对话框，点`OK`关掉。

重新再打开`msys2.exe`，再运行一次上面的命令才能完成更新。

3.安装编译工具

依次运行，有问`[Y/n]`之类的，敲`y`就可以了。

```
pacman -S gcc
```
```
pacman -S mingw-w64-x86_64-toolchain
```
```
pacman -S make
```
4.下载GSL源代码

从官网下载最新版本，[ftp://ftp.gnu.org/gnu/gsl/](ftp://ftp.gnu.org/gnu/gsl/)，当前最新版本为`gsl-2.5.tar.gz`。解压到`c`盘，`c:\gsl-2.5`。

5.开始编译GSL

进入`c:\msys64`，双击运行`mingw64.exe`。在命令行下，依次运行以下每行命令：
```
cd /c/gsl-2.5
./configure CFLAGS="-m64" --prefix=/c/gsl-2.5/_build
make
make install
```
第二行和第三行的命令花的时间会长一点。

运行完之后，会在`c:\gsl-2.5\_build`下生成编译好的库。

如果要生成32位的，在运行第二行命令的时候，改成
```
./configure --prefix=/c/gsl-2.5/_build
```

源码可于github下载：[https://github.com/gkimeeq/OriginAdvancedApplication](https://github.com/gkimeeq/OriginAdvancedApplication)。
