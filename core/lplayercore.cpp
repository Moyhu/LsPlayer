#include "lplayercore.h"

struct SwsContext *sws_ctx = NULL;
unsigned char* dst_data[4];
int dst_linesize[4];
AVFrame *pFrameRGB;
unsigned char* out_buffer;

LPlayerCore::LPlayerCore()
    : m_fmt_ctx(NULL),
      m_video_dec_ctx(NULL),
      m_audio_dec_ctx(NULL),
      m_video_stream(NULL),
      m_audio_stream(NULL),
      m_video_stream_idx(-1),
      m_audio_stream_idx(-1)
{

}

LPlayerCore::~LPlayerCore()
{}

void LPlayerCore::registerCallback(LPlayerCallback *cb)
{
    m_cb = cb;
    m_io.registerCallback(cb);
}

void LPlayerCore::initialize()
{
    m_ioQueue = new LVessel(sizeof(AVPacket), 20);
    m_decodeQueue = new LVessel(sizeof(AVFrame*), 3);
}

void LPlayerCore::setMediaURL(char* url)
{
    m_mediaURL = url;
    /* open input file, and allocate format context */
    if (avformat_open_input(&m_fmt_ctx, url, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", url);
        exit(1);
    }

    /* retrieve stream information */
    if (avformat_find_stream_info(m_fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }

    /* find video codec*/
    if (openCodecContext(&m_video_stream_idx, &m_video_dec_ctx, m_fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0)
    {
        m_video_stream = m_fmt_ctx->streams[m_video_stream_idx];

        m_width = m_video_dec_ctx->width;
        m_height = m_video_dec_ctx->height;
        m_pix_fmt = m_video_dec_ctx->pix_fmt;
    }

    /* find audio codec*/
    if (openCodecContext(&m_audio_stream_idx, &m_audio_dec_ctx, m_fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0)
    {
        m_audio_stream = m_fmt_ctx->streams[m_audio_stream_idx];
    }

    /* dump input information to stderr */
    av_dump_format(m_fmt_ctx, 0, m_fmt_ctx->filename, 0);

    if (!m_audio_stream && !m_video_stream) {
        fprintf(stderr, "Could not find audio or video stream in the input, aborting\n");
    }

    // 以下应放进线程中运行
    printf("audio codec_id: %d\n", m_audio_stream->codec->codec_id);
    printf("audio codec_name: %s\n", avcodec_get_name(m_audio_stream->codec->codec_id));
    printf("audio sample_rate: %d\n", m_audio_stream->codec->sample_rate);
    printf("audio channels: %d\n", m_audio_stream->codec->channels);
    printf("audio sample_fmt: %d\n", m_audio_stream->codec->sample_fmt);
    printf("audio frame_size: %d\n", m_audio_stream->codec->frame_size);
    printf("audio nb_frames: %lld\n", m_audio_stream->nb_frames);
    printf("audio bitrate %lld kb/s\n", (int64_t) m_audio_stream->codec->bit_rate / 1000);

//    m_cb->onParseAudioInfo(m_audio_stream->codec->sample_rate, m_audio_stream->codec->sample_fmt, m_audio_stream->codec->channels, 0, 4096);

    int ret = 0, got_frame;
    video_frame_count = 0;
    audio_frame_count = 0;
    refcount = 0;
    frame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    /* read frames from the file */
    while (av_read_frame(m_fmt_ctx, &pkt) >= 0) {
        AVPacket orig_pkt = pkt;
        do {
            ret = decode_packet(&got_frame, 0);
            if (ret < 0)
                break;
            pkt.data += ret;
            pkt.size -= ret;
        } while (pkt.size > 0);
        av_packet_unref(&orig_pkt);
    }

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame, 1);
    } while (got_frame);

    printf("Demuxing succeeded.\n");

}

extern bool hasdata;
int LPlayerCore::decode_packet(int *got_frame, int cached)
{
    int ret = 0;
    int decoded = pkt.size;

    *got_frame = 0;

    if (pkt.stream_index == m_video_stream_idx) {
        /* decode video frame */
        ret = avcodec_decode_video2(m_video_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
//            fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
            fprintf(stderr, "Error decoding video frame ()\n");
            return ret;
        }

        if (*got_frame) {

            if (frame->width != m_width || frame->height != m_height ||
                frame->format != m_pix_fmt) {
                /* To handle this change, one could call av_image_alloc again and
                 * decode the following frames into another rawvideo file. */
                fprintf(stderr, "Error: Width, height and pixel format have to be "
                        "constant in a rawvideo file, but the width, height or "
                        "pixel format of the input video changed:\n"
                        "old: width = %d, height = %d, format = %s\n"
                        "new: width = %d, height = %d, format = %s\n",
                        m_width, m_height, av_get_pix_fmt_name(m_pix_fmt),
                        frame->width, frame->height,
                        av_get_pix_fmt_name((AVPixelFormat)frame->format));
                return -1;
            }

//            printf("video_frame%s n:%d coded_n:%d pix_format:%s\n",
//                   cached ? "(cached)" : "",
//                   video_frame_count++, frame->coded_picture_number,
//                   av_get_pix_fmt_name((AVPixelFormat)frame->format));


            if (sws_ctx == NULL)
            {
//                sws_ctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format,
//                                             1900,976, AV_PIX_FMT_RGB24,
//                                             SWS_BILINEAR, NULL, NULL, NULL);

//                int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, 1900,976);
//                out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
//                avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB24,
//                        1900, 976);
                sws_ctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format,
                                             1920,1080, AV_PIX_FMT_YUV420P,
                                             SWS_BILINEAR, NULL, NULL, NULL);

                int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, 1920,1080);
                out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
                avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_YUV420P,
                        1920, 1080);
            }

            sws_scale(sws_ctx, (const uint8_t * const*)frame->data,
                              frame->linesize, 0, frame->height, pFrameRGB->data, pFrameRGB->linesize);

//            m_cb->onRanderImage(out_buffer, 1900, 976);
            m_cb->onRanderYUV(pFrameRGB->data, pFrameRGB->linesize);

        }
    } else if (pkt.stream_index == m_audio_stream_idx) {
        /* decode audio frame */
        ret = avcodec_decode_audio4(m_audio_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
//            fprintf(stderr, "Error decoding audio frame (%s)\n", av_err2str(ret));
            fprintf(stderr, "Error decoding audio frame ()\n");
            return ret;
        }
        /* Some audio decoders decode only part of the packet, and have to be
         * called again with the remainder of the packet data.
         * Sample: fate-suite/lossless-audio/luckynight-partial.shn
         * Also, some decoders might over-read the packet. */
        decoded = FFMIN(ret, pkt.size);

        if (*got_frame) {
            size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)frame->format);
//            printf("audio_frame%s n:%d nb_samples:%d pts:%s\n",
//                   cached ? "(cached)" : "",
//                   audio_frame_count++, frame->nb_samples,
//                   av_ts2timestr(frame->pts, &m_audio_dec_ctx->time_base));
            printf("audio_frame%s n:%d nb_samples:%d\n",
                   cached ? "(cached)" : "",
                   audio_frame_count++, frame->nb_samples);

//            while (hasdata)
//            {

//            }
            m_cb->onRanderAudio(frame->extended_data[0], unpadded_linesize);

        }
    }

    /* If we use frame reference counting, we own the data and need
     * to de-reference it when we don't use it anymore */
    if (*got_frame && refcount)
        av_frame_unref(frame);

    return decoded;
}

int LPlayerCore::openCodecContext(int *stream_idx, AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret, stream_index;
    AVStream *st;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), m_mediaURL.data());
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders, with or without reference counting */
//        av_dict_set(&opts, "refcounted_frames", refcount ? "1" : "0", 0);
//        if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
//            fprintf(stderr, "Failed to open %s codec\n",
//                    av_get_media_type_string(type));
//            return ret;
//        }
        if ((ret = avcodec_open2(*dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }

    return 0;
}

void initIOQueue(int index, unsigned char *data, int size)
{
    AVPacket *pkt = (AVPacket*)data;
    av_init_packet(pkt);
    pkt->data = NULL;
    pkt->size = 0;
}

void destoryIOQueue(int index, unsigned char *data, int size)
{
    AVPacket *pkt = (AVPacket*)data;
    // AVPacket 不需要销毁？？
    pkt->data = NULL;
    pkt->size = 0;
}

void initDecodeQueue(int index, unsigned char *data, int size)
{
    AVFrame **frame = (AVFrame**)data;
    *frame = av_frame_alloc();
    if (!*frame) {
        fprintf(stderr, "Index: %d Could not allocate frame\n", index);
    }
}

void destoryDecodeQueue(int index, unsigned char *data, int size)
{
    AVFrame **frame = (AVFrame**)data;
    av_frame_free(frame);
}
