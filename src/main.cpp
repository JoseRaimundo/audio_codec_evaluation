#include <string.h>
#include <string>


using namespace std;

void initialize(string audio){

}


// int initialize_encoding_audio(const char *filename)
// {
//     int ret;
//     AVCodecID aud_codec_id = AV_CODEC_ID_AAC;
//     AVSampleFormat sample_fmt = AV_SAMPLE_FMT_FLTP;

//     avcodec_register_all();
//     av_register_all();

//     aud_codec = avcodec_find_encoder(aud_codec_id);
//     avcodec_register(aud_codec);

//     if (!aud_codec)
//         return COULD_NOT_FIND_AUD_CODEC;

//     aud_codec_context = avcodec_alloc_context3(aud_codec);
//     if (!aud_codec_context)
//         return CONTEXT_CREATION_ERROR;

//     aud_codec_context->bit_rate = 192000;
//     aud_codec_context->sample_rate = select_sample_rate(aud_codec);
//     aud_codec_context->sample_fmt = sample_fmt;
//     aud_codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
//     aud_codec_context->channels = av_get_channel_layout_nb_channels(aud_codec_context->channel_layout);

//     aud_codec_context->codec = aud_codec;
//     aud_codec_context->codec_id = aud_codec_id;

//     ret = avcodec_open2(aud_codec_context, aud_codec, NULL);

//     if (ret < 0)
//         return COULD_NOT_OPEN_AUD_CODEC;

//     outctx = avformat_alloc_context();
//     ret = avformat_alloc_output_context2(&outctx, NULL, "mp4", filename);

//     outctx->audio_codec = aud_codec;
//     outctx->audio_codec_id = aud_codec_id;

//     audio_st = avformat_new_stream(outctx, aud_codec);

//     audio_st->codecpar->bit_rate = aud_codec_context->bit_rate;
//     audio_st->codecpar->sample_rate = aud_codec_context->sample_rate;
//     audio_st->codecpar->channels = aud_codec_context->channels;
//     audio_st->codecpar->channel_layout = aud_codec_context->channel_layout;
//     audio_st->codecpar->codec_id = aud_codec_id;
//     audio_st->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
//     audio_st->codecpar->format = sample_fmt;
//     audio_st->codecpar->frame_size = aud_codec_context->frame_size;
//     audio_st->codecpar->block_align = aud_codec_context->block_align;
//     audio_st->codecpar->initial_padding = aud_codec_context->initial_padding;

//     outctx->streams = new AVStream*[1];
//     outctx->streams[0] = audio_st;

//     av_dump_format(outctx, 0, filename, 1);

//     if (!(outctx->oformat->flags & AVFMT_NOFILE))
//     {
//         if (avio_open(&outctx->pb, filename, AVIO_FLAG_WRITE) < 0)
//             return COULD_NOT_OPEN_FILE;
//     }

//     ret = avformat_write_header(outctx, NULL);

//     aud_frame = av_frame_alloc();
//     aud_frame->nb_samples = aud_codec_context->frame_size;
//     aud_frame->format = aud_codec_context->sample_fmt;
//     aud_frame->channel_layout = aud_codec_context->channel_layout;

//     int buffer_size = av_samples_get_buffer_size(NULL, aud_codec_context->channels, aud_codec_context->frame_size,
//         aud_codec_context->sample_fmt, 0);

//     av_frame_get_buffer(aud_frame, buffer_size / aud_codec_context->channels);

//     if (!aud_frame)
//         return COULD_NOT_ALLOCATE_FRAME;

//     aud_frame_counter = 0;

//     return 0;
// }

// int encode_audio_samples(uint8_t **aud_samples)
// {
//     int ret;

//     int buffer_size = av_samples_get_buffer_size(NULL, aud_codec_context->channels, aud_codec_context->frame_size,
//         aud_codec_context->sample_fmt, 0);

//     for (size_t i = 0; i < buffer_size / aud_codec_context->channels; i++)
//     {
//         aud_frame->data[0][i] = aud_samples[0][i];
//         aud_frame->data[1][i] = aud_samples[1][i];
//     }

//     aud_frame->pts = aud_frame_counter++;

//     ret = avcodec_send_frame(aud_codec_context, aud_frame);
//     if (ret < 0)
//         return ERROR_ENCODING_SAMPLES_SEND;

//     AVPacket pkt;
//     av_init_packet(&pkt);
//     pkt.data = NULL;
//     pkt.size = 0;

//     fflush(stdout);

//     while (true)
//     {
//         ret = avcodec_receive_packet(aud_codec_context, &pkt);
//         if (!ret)
//         {
//             av_packet_rescale_ts(&pkt, aud_codec_context->time_base, audio_st->time_base);

//             pkt.stream_index = audio_st->index;
//             av_write_frame(outctx, &pkt);
//             av_packet_unref(&pkt);
//         }
//         if (ret == AVERROR(EAGAIN))
//             break;
//         else if (ret < 0)
//             return ERROR_ENCODING_SAMPLES_RECEIVE;
//         else
//             break;
//     }

//     return 0;
// }


// int finish_audio_encoding()
// {
//     AVPacket pkt;
//     av_init_packet(&pkt);
//     pkt.data = NULL;
//     pkt.size = 0;

//     fflush(stdout);

//     int ret = avcodec_send_frame(aud_codec_context, NULL);
//     if (ret < 0)
//         return ERROR_ENCODING_FRAME_SEND;

//     while (true)
//     {
//         ret = avcodec_receive_packet(aud_codec_context, &pkt);
//         if (!ret)
//         {
//             if (pkt.pts != AV_NOPTS_VALUE)
//                 pkt.pts = av_rescale_q(pkt.pts, aud_codec_context->time_base, audio_st->time_base);
//             if (pkt.dts != AV_NOPTS_VALUE)
//                 pkt.dts = av_rescale_q(pkt.dts, aud_codec_context->time_base, audio_st->time_base);

//             av_write_frame(outctx, &pkt);
//             av_packet_unref(&pkt);
//         }
//         if (ret == -AVERROR(AVERROR_EOF))
//             break;
//         else if (ret < 0)
//             return ERROR_ENCODING_FRAME_RECEIVE;
//     }

//     av_write_trailer(outctx);
// }

// void get_audio_frame(float_t *left_samples, float_t *right_samples, int frame_size, float* t, float* tincr, float* tincr2)
// {
//     int j, i;
//     float v;
//     for (j = 0; j < frame_size; j++)
//     {
//         v = sin(*t);
//         *left_samples = v;
//         *right_samples = v;

//         left_samples++;
//         right_samples++;

//         *t += *tincr;
//         *tincr += *tincr2;
//     }
// }

// int main()
// {
//     int frame_rate = 30;  // this should be like 96000 / 1024 or somthing i guess?
//     float t, tincr, tincr2;

//     initialize_encoding_audio("audio.mp4");

//     int sec = 50;

//     float_t** aud_samples;
//     int src_samples_linesize;
//     int src_nb_samples = 1024;
//     int src_channels = 2;

//     int ret = av_samples_alloc_array_and_samples((uint8_t***)&aud_samples, &src_samples_linesize, src_channels,
//         src_nb_samples, AV_SAMPLE_FMT_FLTP, 0);


//     t = 0;
//     tincr = 0;
//     tincr2 = 0;

//     for (size_t i = 0; i < frame_rate * sec; i++)
//     {
//         get_audio_frame(aud_samples[0], aud_samples[1], src_nb_samples, &t, &tincr, &tincr2);

//         encode_audio_samples((uint8_t **)aud_samples);

//     }

//     finish_audio_encoding();
//     //cleanup();

//     return 0;
// }


int main(int argc, char const *argv[]){
    int frame_rate = 30;
    initialize("audio.mp3");
    
    return 0;
}

