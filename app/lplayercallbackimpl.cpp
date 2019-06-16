#include "lplayercallbackimpl.h"

LPlayerCallbackImpl::LPlayerCallbackImpl()
{

}

void LPlayerCallbackImpl::onRanderImage(unsigned char *data, int w, int h)
{
    emit RanderImage(data, w, h);
}

void LPlayerCallbackImpl::onRanderYUV(unsigned char **data, int *linesize)
{
    emit RanderYUV(data, linesize);
}

char *pcm_buffer = NULL;
int buffer_size = 4096;
uint8_t *audio_pos;
uint32_t audio_len;
bool hasdata = false;
void read_audio_data(void *udata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_len == 0)
        return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;

    if (audio_len == 0)
    {
        hasdata = false;
    }
}

void LPlayerCallbackImpl::onParseAudioInfo(int freq, uint16_t format, uint8_t channels, uint8_t silence, uint16_t samples)
{

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return ;
    }

    SDL_AudioSpec spec;
    spec.freq = 48000;//根据你录制的PCM采样率决定
    spec.format = AUDIO_S32SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = spec.format * spec.channels;
    spec.callback = read_audio_data;
    spec.userdata = NULL;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("can't open audio.\n");
        return;
    }

    pcm_buffer = new char[buffer_size];
    //播放
    SDL_PauseAudio(0);
}

void LPlayerCallbackImpl::onRanderAudio(unsigned char *data, uint32_t size)
{
    hasdata = true;
    audio_len = size;
    audio_pos = data;
}
