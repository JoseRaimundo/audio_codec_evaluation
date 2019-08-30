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