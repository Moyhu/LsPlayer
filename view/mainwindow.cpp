#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <QTime>

#include "core/lplayercore.h"
#include "app/lplayercallbackimpl.h"


LPlayerCallbackImpl* impl;

int screen_w,screen_h;
SDL_Window *screen;
SDL_Renderer* sdlRenderer;
SDL_Texture* sdlTexture;
SDL_Rect sdlRect;
SDL_Thread *video_tid;
SDL_Event event;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    impl = new LPlayerCallbackImpl();


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf( "Could not initialize SDL - %s\n", SDL_GetError());
    }
    //SDL 2.0 Support for multiple windows
    screen_w = 1920;
    screen_h = 1080;
//    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//        screen_w, screen_h,SDL_WINDOW_OPENGL);
    screen = SDL_CreateWindowFrom((void*)ui->lblPicture->winId());


    if(!screen) {
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,screen_w,screen_h);

    sdlRect.x=0;
    sdlRect.y=0;
    sdlRect.w=screen_w;
    sdlRect.h=screen_h;




    connect(impl, SIGNAL(RanderImage(unsigned char*, int, int)), this, SLOT(onRanderImage(unsigned char*, int, int)));
    connect(impl, SIGNAL(RanderYUV(unsigned char**, int*)), this, SLOT(onRanderYUV(unsigned char**, int*)));
}

MainWindow::~MainWindow()
{
    SDL_Quit();
    delete ui;
}

void MainWindow::onRanderImage(unsigned char *data, int w, int h)
{

    QPixmap pix = QPixmap::fromImage(QImage(data,w,h,QImage::Format_RGB888));
    ui->lblPicture->setPixmap(pix);
    QTime dieTime = QTime::currentTime().addMSecs(30);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::onRanderYUV(unsigned char **data, int *linesize)
{
    SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
                    data[0], linesize[0],
                    data[1], linesize[1],
                    data[2], linesize[2]);

    SDL_RenderClear( sdlRenderer );
    SDL_RenderCopy( sdlRenderer, sdlTexture,  NULL, &sdlRect);
    SDL_RenderPresent( sdlRenderer );
//    SDL_Delay(30);
    QTime dieTime = QTime::currentTime().addMSecs(30);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_btnPlay_clicked()
{
    LPlayerCore player;
    player.registerCallback(impl);
    player.initialize();
//    player.setMediaURL("../LsPlayer/res/media/ad_mini.mp4");
    player.setMediaURL("../LsPlayer/res/media/output.mkv");
}
