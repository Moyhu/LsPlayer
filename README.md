# LsPlayer
一步一步打造万能播放器，预备使用FFmpeg解码，SDL渲染图像与音频，QT做界面框架

编译问题
1、#error missing -D__STDC_FORMAT_MACROS / #define __STDC_FORMAT_MACROS
c++使用PRID64，需要两步：
1.包含头文件：<inttypes.h>
2.定义宏：__STDC_FORMAT_MACROS，可以通过编译时加-D__STDC_FORMAT_MACROS，或者在包含文件之前定义这个宏。

2、error: taking address of temporary array
引用了临时对象的地址而引发的警报
暂不知道如何解决，修改ffmpeg代码？
临时方法：避免使用有该问题的宏（大多是用于打印信息）