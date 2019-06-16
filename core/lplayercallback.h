#ifndef LPLAYERCALLBACK_H
#define LPLAYERCALLBACK_H

#include "stdint.h"

class LPlayerCallback
{
public:
    virtual void onRanderImage(unsigned char* data, int w, int h) = 0;

    virtual void onRanderYUV(unsigned char **data, int *linesize) = 0;

    virtual void onParseAudioInfo(int freq, uint16_t format, uint8_t channels, uint8_t silence, uint16_t samples) = 0;

    virtual void onRanderAudio(unsigned char *data, uint32_t size) = 0;
};

#endif // LPLAYERCALLBACK_H
