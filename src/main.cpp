#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>



extern "C" {
    #ifndef __STDC_CONSTANT_MACROS
    #define __STDC_CONSTANT_MACROS
    #endif

    #include <libavutil/samplefmt.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/common.h>
    #include <math.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}


using namespace std;

 
int main(int argc, char *argv[]){

    av_register_all();
    // avdevice_register_all();
    avcodec_register_all();
    avformat_network_init();
 
    const char  *filenameSrc = "rtp://127.0.0.1:1234";
 
    AVCodecContext  *pCodecCtx;
    AVFormatContext *pFormatCtx = avformat_alloc_context();
 
    AVCodec * pCodec;
    cout << "Aguardando 0" << endl;
 
    if(avformat_open_input(&pFormatCtx,filenameSrc,NULL,NULL) != 0) return -12;
    cout << "Aguardando 1" << endl;

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)   return -13;
    av_dump_format(pFormatCtx, 0, filenameSrc, 0);
    
    int videoStream = 1;
    cout << "Aguardando 2" << endl;

    for(int i=0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO){
            videoStream = i;
            break;
        }
    }
    cout << "Aguardando 3" << endl;
 
    if(videoStream == -1) return -14;
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
 
    pCodec =avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) return -15; //codec not found
 
    if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0) return -16;

    uint8_t *buffer;
    int numBytes;
 

    int res;
    int frameFinished;
    AVPacket packet;
    while(res = av_read_frame(pFormatCtx,&packet)>=0)   {
        cout << "Algo chegando " << endl;
        if(packet.stream_index == videoStream){
            // if(frameFinished){
            //     struct SwsContext * img_convert_ctx;
            //     img_convert_ctx = sws_getCachedContext(NULL,pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,   pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL,NULL);
            //     sws_scale(img_convert_ctx, ((AVPicture*)pFrame)->data, ((AVPicture*)pFrame)->linesize, 0, pCodecCtx->height, ((AVPicture *)pFrameRGB)->data, ((AVPicture *)pFrameRGB)->linesize);
 
            //     // cv::Mat img(pFrame->height,pFrame->width,CV_8UC3,pFrameRGB->data[0]); //dst->data[0]);
            //     // cv::imshow("display",img);
            //     // cvWaitKey(1);
 
            //     av_free_packet(&packet);
            //     sws_freeContext(img_convert_ctx);
                 
            // }
        }
    }
 
    av_free_packet(&packet);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
 
    return 0;
}



























// // static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
// // {
// //     AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

// //     /*printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
// //            av_ts_make_string(pkt->pts), av_ts_make_string(pkt->pts, time_base),
// //            av_ts_make_string(pkt->dts), av_ts_make_string(pkt->dts, time_base),
// //            av_ts_make_string(pkt->duration), av_ts_make_string(pkt->duration, time_base),
// //            pkt->stream_index);*/
// // }

// // static int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
// // {
// //     /* rescale output packet timestamp values from codec to stream timebase */
// //     pkt->pts = av_rescale_q_rnd(pkt->pts, *time_base, st->time_base, AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
// //     pkt->dts = av_rescale_q_rnd(pkt->dts, *time_base, st->time_base, AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
// //     pkt->duration = av_rescale_q(pkt->duration, *time_base, st->time_base);
// //     pkt->stream_index = st->index;

// //     /* Write the compressed frame to the media file. */
// //     log_packet(fmt_ctx, pkt);
// //     return av_interleaved_write_frame(fmt_ctx, pkt);
// // }

// // /* Add an output stream. */
// // static AVStream *add_stream(AVFormatContext *oc, AVCodec **codec,
// //                             enum AVCodecID codec_id)
// // {
// //     AVCodecContext *c;
// //     AVStream *st;

// //     /* find the encoder */
// //     *codec = avcodec_find_encoder(codec_id);
// //     if (!(*codec)) {
// //         fprintf(stderr, "Could not find encoder for '%s'\n",
// //                 avcodec_get_name(codec_id));
// //         exit(1);
// //     }

// //     st = avformat_new_stream(oc, *codec);
// //     if (!st) {
// //         fprintf(stderr, "Could not allocate stream\n");
// //         exit(1);
// //     }
// //     st->id = oc->nb_streams-1;
// //     c = st->codec;

// //     switch ((*codec)->type) {
// //     case AVMEDIA_TYPE_AUDIO:
// //         c->sample_fmt  = (*codec)->sample_fmts ?
// //             (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
// //         c->bit_rate    = 64000;
// //         c->sample_rate = 44100;
// //         c->channels    = 2;
// //         break;

// //     case AVMEDIA_TYPE_VIDEO:
// //         c->codec_id = codec_id;
// //         if(codec_id == CODEC_ID_H264)
// //             cout<<"Codec ID  "<<(AVCodecID)codec_id<<endl;
// //         c->bit_rate = 400000;
// //         /* Resolution must be a multiple of two. */
// //         c->width    = 352;
// //         c->height   = 288;
// //         /* timebase: This is the fundamental unit of time (in seconds) in terms
// //          * of which frame timestamps are represented. For fixed-fps content,
// //          * timebase should be 1/framerate and timestamp increments should be
// //          * identical to 1. */
// //         c->time_base.den = STREAM_FRAME_RATE;
// //         c->time_base.num = 1;
// //         c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
// //         c->pix_fmt       = STREAM_PIX_FMT;
// //         if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
// //             /* just for testing, we also add B frames */
// //             c->max_b_frames = 2;
// //         }
// //         if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
// //             /* Needed to avoid using macroblocks in which some coeffs overflow.
// //              * This does not happen with normal video, it just happens here as
// //              * the motion of the chroma plane does not match the luma plane. */
// //             c->mb_decision = 2;
// //         }
// //     break;

// //     default:
// //         break;
// //     }

// //     /* Some formats want stream headers to be separate. */
// //     if (oc->oformat->flags & AVFMT_GLOBALHEADER)
// //         c->flags |= CODEC_FLAG_GLOBAL_HEADER;

// //     return st;
// // }

// // /**************************************************************/
// // /* audio output */

// // static float t, tincr, tincr2;

// // AVFrame *audio_frame;
// // static uint8_t **src_samples_data;
// // static int       src_samples_linesize;
// // static int       src_nb_samples;

// // static int max_dst_nb_samples;
// // uint8_t **dst_samples_data;
// // int       dst_samples_linesize;
// // int       dst_samples_size;
// // int samples_count;

// // struct SwrContext *swr_ctx = NULL;

// // static void open_audio(AVFormatContext *oc, AVCodec *codec, AVStream *st)
// // {
// //     AVCodecContext *c;
// //     int ret;

// //     c = st->codec;

// //     /* allocate and init a re-usable frame */
// //     audio_frame = av_frame_alloc();
// //     if (!audio_frame) {
// //         fprintf(stderr, "Could not allocate audio frame\n");
// //         exit(1);
// //     }

// //     /* open it */
// //     ret = avcodec_open2(c, codec, NULL);
// //     if (ret < 0) {
// //         fprintf(stderr, "Could not open audio codec:\n");
// //         exit(1);
// //     }

// //     /* init signal generator */
// //     t     = 0;
// //     tincr = 2 * M_PI * 110.0 / c->sample_rate;
// //     /* increment frequency by 110 Hz per second */
// //     tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;

// //     src_nb_samples = c->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE ?
// //         10000 : c->frame_size;

// //     ret = av_samples_alloc_array_and_samples(&src_samples_data, &src_samples_linesize, c->channels,
// //                                              src_nb_samples, AV_SAMPLE_FMT_S16, 0);
// //     if (ret < 0) {
// //         fprintf(stderr, "Could not allocate source samples\n");
// //         exit(1);
// //     }

// //     /* compute the number of converted samples: buffering is avoided
// //      * ensuring that the output buffer will contain at least all the
// //      * converted input samples */
// //     max_dst_nb_samples = src_nb_samples;

// //     /* create resampler context */
// //     if (c->sample_fmt != AV_SAMPLE_FMT_S16) {
// //         swr_ctx = swr_alloc();
// //         if (!swr_ctx) {
// //             fprintf(stderr, "Could not allocate resampler context\n");
// //             exit(1);
// //         }

// //         /* set options */
// //         av_opt_set_int       (swr_ctx, "in_channel_count",   c->channels,       0);
// //         av_opt_set_int       (swr_ctx, "in_sample_rate",     c->sample_rate,    0);
// //         av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
// //         av_opt_set_int       (swr_ctx, "out_channel_count",  c->channels,       0);
// //         av_opt_set_int       (swr_ctx, "out_sample_rate",    c->sample_rate,    0);
// //         av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);

// //         /* initialize the resampling context */
// //         if ((ret = swr_init(swr_ctx)) < 0) {
// //             fprintf(stderr, "Failed to initialize the resampling context\n");
// //             exit(1);
// //         }

// //         ret = av_samples_alloc_array_and_samples(&dst_samples_data, &dst_samples_linesize, c->channels,
// //                                                  max_dst_nb_samples, c->sample_fmt, 0);
// //         if (ret < 0) {
// //             fprintf(stderr, "Could not allocate destination samples\n");
// //             exit(1);
// //         }
// //     } else {
// //         dst_samples_data = src_samples_data;
// //     }
// //     dst_samples_size = av_samples_get_buffer_size(NULL, c->channels, max_dst_nb_samples,
// //                                                   c->sample_fmt, 0);
// // }

// // /* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
// //  * 'nb_channels' channels. */
// // static void get_audio_frame(int16_t *samples, int frame_size, int nb_channels)
// // {
// //     int j, i, v;
// //     int16_t *q;

// //     q = samples;
// //     for (j = 0; j < frame_size; j++) {
// //         v = (int)(sin(t) * 10000);
// //         for (i = 0; i < nb_channels; i++)
// //             *q++ = v;
// //         t     += tincr;
// //         tincr += tincr2;
// //     }
// // }

// // static void write_audio_frame(AVFormatContext *oc, AVStream *st, int flush)
// // {
// //     AVCodecContext *c;
// //     AVPacket pkt = { 0 }; // data and size must be 0;
// //     int got_packet, ret, dst_nb_samples;

// //     av_init_packet(&pkt);
// //     c = st->codec;

// //     if (!flush) {
// //         get_audio_frame((int16_t *)src_samples_data[0], src_nb_samples, c->channels);

// //         /* convert samples from native format to destination codec format, using the resampler */
// //         if (swr_ctx) {
// //             /* compute destination number of samples */
// //             dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, c->sample_rate) + src_nb_samples,
// //                                             c->sample_rate, c->sample_rate, AV_ROUND_UP);
// //             if (dst_nb_samples > max_dst_nb_samples) {
// //                 av_free(dst_samples_data[0]);
// //                 ret = av_samples_alloc(dst_samples_data, &dst_samples_linesize, c->channels,
// //                                        dst_nb_samples, c->sample_fmt, 0);
// //                 if (ret < 0)
// //                     exit(1);
// //                 max_dst_nb_samples = dst_nb_samples;
// //                 dst_samples_size = av_samples_get_buffer_size(NULL, c->channels, dst_nb_samples,
// //                                                               c->sample_fmt, 0);
// //             }

// //             /* convert to destination format */
// //             ret = swr_convert(swr_ctx,
// //                               dst_samples_data, dst_nb_samples,
// //                               (const uint8_t **)src_samples_data, src_nb_samples);
// //             if (ret < 0) {
// //                 fprintf(stderr, "Error while converting\n");
// //                 exit(1);
// //             }
// //         } else {
// //             dst_nb_samples = src_nb_samples;
// //         }
// //         AVRational t= {1, c->sample_rate};
// //         audio_frame->nb_samples = dst_nb_samples;
// //         audio_frame->pts = av_rescale_q(samples_count,t , c->time_base);
// //         avcodec_fill_audio_frame(audio_frame, c->channels, c->sample_fmt,
// //                                  dst_samples_data[0], dst_samples_size, 0);
// //         samples_count += dst_nb_samples;
// //     }

// //     ret = avcodec_encode_audio2(c, &pkt, flush ? NULL : audio_frame, &got_packet);
// //     if (ret < 0) {
// //         fprintf(stderr, "Error encoding audio frame:");
// //         exit(1);
// //     }

// //     if (!got_packet) {
// //         if (flush)
// //             audio_is_eof = 1;
// //         return;
// //     }

// //     ret = write_frame(oc, &c->time_base, st, &pkt);
// //     if (ret < 0) {
// //         fprintf(stderr, "Error while writing audio frame: ");
// //         exit(1);
// //     }
// // }

// // static void close_audio(AVFormatContext *oc, AVStream *st)
// // {
// //     avcodec_close(st->codec);
// //     if (dst_samples_data != src_samples_data) {
// //         av_free(dst_samples_data[0]);
// //         av_free(dst_samples_data);
// //     }
// //     av_free(src_samples_data[0]);
// //     av_free(src_samples_data);
// //     av_frame_free(&audio_frame);
// // }

// /**************************************************************/
// /* media file output */

// int main(int argc, char **argv){

//     const char *filename = "rtsp://127.0.0.1:8554/live.sdp";
//     AVOutputFormat *fmt;
//     AVFormatContext *oc;
//     AVStream *audio_st;
//     AVCodec *audio_codec;
//     double audio_time;
//     int flush, ret;

//     /* Initialize libavcodec, and register all codecs and formats. */
//     av_register_all();
//     avformat_network_init();


//     /* allocate the output media context */
//     avformat_alloc_output_context2(&oc, NULL, "rtsp", filename);

//     if (!oc) {
//         printf("Could not deduce output format from file extension: using MPEG.\n");
//         avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
//     }

//     if (!oc)
//         return 1;

//     fmt = oc->oformat;
//     if(!fmt)
//         cout<<"Error creating outformat\n";
//     /* Add the audio and video streams using the default format codecs
//      * and initialize the codecs. */

//     /*if(av_opt_set(fmt,"rtsp_transport","tcp",0) < 0)
//         cout<<"Opt not set\n";
//     // audio_st = NULL;
//     // cout<<"Codec = "<<avcodec_get_name(fmt->video_codec)<<endl;
 
//     // if (fmt->audio_codec != AV_CODEC_ID_NONE)
//     //     audio_st = add_stream(oc, &audio_codec, fmt->audio_codec);

//     // /* Now that all the parameters are set, we can open the audio and
//     //  * video codecs and allocate the necessary encode buffers. */

//     // if (audio_st)
//     //     open_audio(oc, audio_codec, audio_st);

//     // av_dump_format(oc, 0, filename, 1);
//     // char errorBuff[80];

//     // if (!(fmt->flags & AVFMT_NOFILE)) {
//     //     ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
//     //     if (ret < 0) {
//     //         fprintf(stderr, "Could not open outfile '%s': %s",filename,av_make_error_string(errorBuff,80,ret));
//     //         return 1;
//     //     }
//     // }


//     // ret = avformat_write_header(oc, NULL);
//     // if (ret < 0) {
//     //     fprintf(stderr, "Error occurred when writing header: %s",av_make_error_string(errorBuff,80,ret));
//     //     return 1;
//     // }

//     // flush = 0;
//     // while (audio_st && !audio_is_eof) {

//     //     /* Compute current audio and video time. */
//     //     audio_time = (audio_st && !audio_is_eof) ? audio_st->pts.val * av_q2d(audio_st->time_base) : INFINITY;

//     //     if (!flush &&
//     //         (!audio_st || audio_time >= STREAM_DURATION)) {
//     //         flush = 1;
//     //     }

//     //     /* write interleaved audio and video frames */
//     //     if (audio_st && !audio_is_eof && audio_time) {
//     //         write_audio_frame(oc, audio_st, flush);
//     //     } 
//     // }

//     // /* Write the trailer, if any. The trailer must be written before you
//     //  * close the CodecContexts open when you wrote the header; otherwise
//     //  * av_write_trailer() may try to use memory that was freed on
//     //  * av_codec_close(). */
//     // av_write_trailer(oc);

//     // /* Close each codec. */

//     // if (audio_st)
//     //     close_audio(oc, audio_st);

//     // if (!(fmt->flags & AVFMT_NOFILE))
//     //     avio_close(oc->pb);

//     // /* free the stream */
//     // avformat_free_context(oc);

//     return 0;
// }


