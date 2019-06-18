#-------------------------------------------------
#
# Project created by QtCreator 2019-06-12T21:12:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LsPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

QMAKE_CXXFLAGS += \
                -D__STDC_FORMAT_MACROS \
                -fpermissive

SOURCES += \
        core/iothread.cpp \
        core/lplayercore.cpp \
        lutil/lthread.cpp \
        lutil/lvessel.cpp \
        app/main.cpp \
        app/lplayercallbackimpl.cpp \
        view/mainwindow.cpp

HEADERS += \
        core/iothread.h \
        core/lplayercallback.h \
        core/lplayercore.h \
        lutil/lthread.h \
        lutil/lvessel.h \
        app/lplayercallbackimpl.h \
        view/mainwindow.h

FORMS += \
        view/mainwindow.ui

INCLUDEPATH += \
        $$PWD/thirdparty/ffmpeg-20190529-d903c09-win64-dev/include \
        $$PWD/thirdparty/sdl-x86_64-w64-mingw32/include

LIBS += \
        -L$$PWD/thirdparty/ffmpeg-20190529-d903c09-win64-dev/lib \
        -L$$PWD/thirdparty/sdl-x86_64-w64-mingw32/lib \
        -lavformat \
        -lavdevice \
        -lavcodec \
        -lswscale \
        -lavutil \
        -lswresample \
        -lm \
        -lz \
        -lSDL2 \
        -lSDL2main \
        -lSDL2_test

DEFINES += \
        QT_NO_WARNING_OUTPUT \
        QT_NO_DEBUG_OUTPUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
