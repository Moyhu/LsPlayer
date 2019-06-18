#include "view/mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "lplayercallbackimpl.h"
#include "lutil/lthread.h"
#include <iostream>
#include <QTime>
extern "C"{
#include <libavcodec/avcodec.h>
#include <SDL2/SDL.h>

#undef main
}

#include "core/lplayercore.h"

void fftest()
{
    qDebug() << avcodec_configuration();
}

void sdltest()
{
    SDL_Window* window = NULL;             // Declare a pointer
    SDL_Renderer* renderer = NULL;         //渲染器
    SDL_Surface* bmp = NULL;

    // 初始化SDL
    // int SDLCALL SDL_Init(Uint32 flags)
    //SDL_INIT_TIMER：定时器
    //SDL_INIT_AUDIO：音频
    //SDL_INIT_VIDEO：视频
    //SDL_INIT_JOYSTICK：摇杆
    //SDL_INIT_HAPTIC：触摸屏
    //SDL_INIT_GAMECONTROLLER：游戏控制器
    //SDL_INIT_EVENTS：事件
    //SDL_INIT_NOPARACHUTE：不捕获关键信号（这个不理解）
    //SDL_INIT_EVERYTHING：包含上述所有选项
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        qDebug() << "SDL_Init Error: " << SDL_GetError();
        return;
    }

    // 使用以下设置创建应用程序窗口:
    // SDL_Window * SDLCALL SDL_CreateWindow(const char *title,int x, int y, int w, int h, Uint32 flags);
    //参数含义如下。
    //title	：窗口标题
    // x	：窗口位置x坐标。也可以设置为SDL_WINDOWPOS_CENTERED或SDL_WINDOWPOS_UNDEFINED。
    // y	：窗口位置y坐标。同上。
    // w	：窗口的宽
    // h	：窗口的高
    // flags ：支持下列标识。包括了窗口的是否最大化、最小化，能否调整边界等等属性。
    // SDL_WINDOW_FULLSCREEN    全屏窗口
    // SDL_WINDOW_FULLSCREEN_DESKTOP  当前桌面分辨率下的全屏窗口
    // SDL_WINDOW_OPENGL    使用OpenGL上下文的窗口
    // SDL_WINDOW_VULKAN  与Vulkan实例一起使用的窗口
    // SDL_WINDOW_HIDDEN    窗口不可见
    // SDL_WINDOW_BORDERLESS  无窗装饰
    // SDL_WINDOW_RESIZABLE  窗口可以调整大小
    // SDL_WINDOW_MINIMIZED   最小化窗口
    // SDL_WINDOW_MAXIMIZED  最大化窗口
    // SDL_WINDOW_INPUT_GRABBED  窗口抓住输入焦点
    // SDL_WINDOW_ALLOW_HIGHDPI  如果支持，窗口应该在高DPI模式下创建（> = SDL2.0.1）
    // SDL_WINDOW_SHOWN  可见窗口
    // SDL_WINDOW_POPUP_MENU  窗口应被视为弹出式菜单（x11，>＝SDL 2.0.5）
    // SDL_WINDOW_TOOLTIP  窗口应被视为工具提示（X11，> = SDL 2.0.5）
    // SDL_WINDOW_UTILITY  窗口应视为实用窗口（x11，>＝SDL 2.0.5）
    // SDL_WINDOW_SKIP_TASKBAR  窗口不应该添加到任务栏（X11，> = SDL 2.0.5）
    // SDL_WINDOW_ALWAYS_ON_TOP  窗口应该总是高于其他（x11，>＝SDL 2.0.5）
    // SDL_WINDOW_MOUSE_CAPTURE  窗口已捕获鼠标（与输入的抓取无关，> = SDL 2.0.4）
    // SDL_WINDOW_FOREIGN  不由SDL创建的窗口
    // SDL_WINDOW_MOUSE_FOCUS   窗口有鼠标焦点
    // SDL_WINDOW_INPUT_FOCUS   窗口有输入焦点
    // 返回创建完成的窗口的ID。如果创建失败则返回0。
    window = SDL_CreateWindow(
        "Hello SDL!!",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        555,                               // width, in pixels
        333,                               // height, in pixels
        SDL_WINDOW_SHOWN                  // flags - see below
    );
    // 检查窗口是否成功创建
    if (window == nullptr)
    {
        qDebug() << "SDL_CreateWindow Error: " << SDL_GetError();
        SDL_Quit();
        return;
    }

    // SDL中使用SDL_CreateRenderer()基于窗口创建渲染器。SDL_CreateRenderer()原型如下。
    // SDL_Renderer * SDLCALL SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags);
    // 参数含义如下。
    // window	： 渲染的目标窗口。
    // index	：打算初始化的渲染设备的索引。设置“-1”则初始化默认的渲染设备。
    // flags	：支持以下值（位于SDL_RendererFlags定义中）
    // SDL_RENDERER_SOFTWARE ：使用软件渲染
    // SDL_RENDERER_ACCELERATED ：使用硬件加速
    // SDL_RENDERER_PRESENTVSYNC：和显示器的刷新率同步
    // SDL_RENDERER_TARGETTEXTURE ：不太懂
    // 返回创建完成的渲染器的ID。如果创建失败则返回NULL。
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // 检查渲染器是否成功创建
    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
        qDebug() << "SDL_CreateRenderer Error: " << SDL_GetError();
        SDL_Quit();
        return;
    }

    //加载图片
    bmp = SDL_LoadBMP("../LsPlayer/res/img/testyuv.bmp");
    if (bmp == nullptr)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        qDebug() << "SDL_LoadBMP Error: " << SDL_GetError();
        SDL_Quit();
        return;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        qDebug() << "SDL_CreateTextureFromSurface Error: " << SDL_GetError();
        SDL_Quit();
        return;
    }

    // 窗口是打开的：可以在这里输入程序循环(see SDL_PollEvent())
    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 3; ++i)
    {
        //First clear the renderer
        SDL_RenderClear(renderer);
        //Draw the texture
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        //Update the screen
        SDL_RenderPresent(renderer);
        //Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    // 关闭并摧毁窗户
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // 清理
    SDL_Quit();
}

class test
{
public:
    test() {}
    void test1(){
        lt.start(&test::go, this, 6);
    }

    void go(int i) {
        std::cout << "start !!" << i << std::endl;
        QString time;
        while (1)
        {
            static int cnt = 0;
            time = QTime::currentTime().toString();
            std::cout << cnt++ << "  "<< time.toStdString() << std::endl;
            LThread::msleep(100);
            if (cnt > 50)
                break;
        }
    }
    LThread lt;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    fftest();
//    sdltest();
    {
    test t;
    t.test1();
    LThread::sleep(5);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
