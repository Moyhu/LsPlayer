#ifndef LPLAYERCORE_H
#define LPLAYERCORE_H

#include "lplayercallback.h"
#include "liomodule.h"
#include "lutil/lvessel.h"
#include <string>

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>
}

using namespace std;

class LPlayerCore
{
public:
    LPlayerCore();

    ~LPlayerCore();

    void registerCallback(LPlayerCallback *cb);

    void initialize();

    void setMediaURL(char* url);

private:

    static void initIOQueue(int, unsigned char *, int);

    static void destoryIOQueue(int, unsigned char *, int);

    static void initDecodeQueue(int, unsigned char *, int);

    static void destoryDecodeQueue(int, unsigned char *, int);

    int openCodecContext(int *stream_idx, AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type);

    int decode_packet(int *got_frame, int cached);

private:
    LPlayerCallback*    m_cb;
    LIOModule           m_io;


private:
    AVFormatContext*    m_fmt_ctx;
    AVCodecContext*     m_video_dec_ctx;
    AVCodecContext*     m_audio_dec_ctx;
    AVStream*           m_video_stream;
    AVStream*           m_audio_stream;
    enum AVPixelFormat  m_pix_fmt;

    string              m_mediaURL;
    int                 m_video_stream_idx;
    int                 m_audio_stream_idx;
    int                 m_width;
    int                 m_height;

    LVessel*            m_ioQueue;
    LVessel*            m_decodeQueue;

public:
    int video_frame_count;
    int audio_frame_count;
    int refcount;

    AVPacket pkt;
    AVFrame* frame;
};

#endif // LPLAYERCORE_H
