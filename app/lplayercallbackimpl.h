#ifndef LPLAYERCALLBACKIMPL_H
#define LPLAYERCALLBACKIMPL_H

#include <QObject>
#include "core/lplayercallback.h"

extern "C" {
#include "SDL2/SDL_types.h"
#include "SDL2/SDL.h"
}

class LPlayerCallbackImpl : public QObject, public LPlayerCallback
{
    Q_OBJECT
public:
    LPlayerCallbackImpl();

    virtual void onRanderImage(unsigned char* data, int w, int h);

    virtual void onRanderYUV(unsigned char **data, int *linesize);

    virtual void onParseAudioInfo(int freq, uint16_t format, uint8_t channels, uint8_t silence, uint16_t samples);

    virtual void onRanderAudio(unsigned char *data, uint32_t size);

signals:
    void RanderImage(unsigned char*, int, int);
    void RanderYUV(unsigned char **, int *);

};

#endif // LPLAYERCALLBACKIMPL_H
