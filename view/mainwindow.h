#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>

#include "SDL2/SDL.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRanderImage(unsigned char*, int, int);
    void onRanderYUV(unsigned char **data, int *linesize);

private slots:
    void on_btnPlay_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
