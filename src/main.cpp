#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <strings.h>
#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include <errno.h>
#include "libavutil/avutil.h"


extern "C" {
         #include <libavcodec/avcodec.h>
         #include <libavformat/avformat.h>
         #include <libswscale/swscale.h>
         #include <libavutil/imgutils.h>
}


#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

using namespace std;



/*
 * Audio encoding example
 */
static void audio_encode_example(const char *filename) {
    // AVCodec *codec;
    // AVCodecContext *c= NULL;
    // int frame_size, i, j, out_size, outbuf_size;
    // FILE *f;
    // short *samples;
    // float t, tincr;
    // uint8_t *outbuf;

    // printf("Audio encoding\n");

    // /* find the MP2 encoder */
    // codec = avcodec_find_encoder(CODEC_ID_MP2);
    // if (!codec) {
    //     fprintf(stderr, "codec not found\n");
    //     exit(1);
    // }

    // c= avcodec_alloc_context();

    // /* put sample parameters */
    // c->bit_rate = 64000;
    // c->sample_rate = 44100;
    // c->channels = 2;

    // /* open it */
    // if (avcodec_open(c, codec) < 0) {
    //     fprintf(stderr, "could not open codec\n");
    //     exit(1);
    // }

    // /* the codec gives us the frame size, in samples */
    // frame_size = c->frame_size;
    // samples = malloc(frame_size * 2 * c->channels);
    // outbuf_size = 10000;
    // outbuf = malloc(outbuf_size);

    // f = fopen(filename, "wb");
    // if (!f) {
    //     fprintf(stderr, "could not open %s\n", filename);
    //     exit(1);
    // }

    // /* encode a single tone sound */
    // t = 0;
    // tincr = 2 * M_PI * 440.0 / c->sample_rate;
    // for(i=0;i<200;i++) {
    //     for(j=0;j<frame_size;j++) {
    //         samples[2*j] = (int)(sin(t) * 10000);
    //         samples[2*j+1] = samples[2*j];
    //         t += tincr;
    //     }
    //     /* encode the samples */
    //     out_size = avcodec_encode_audio(c, outbuf, outbuf_size, samples);
    //     fwrite(outbuf, 1, out_size, f);
    // }
    // fclose(f);
    // free(outbuf);
    // free(samples);

    // avcodec_close(c);
    // av_free(c);
}

// /*
//  * Audio decoding.
//  */
static void audio_decode_example(const char *outfilename, const char *filename){
//     AVCodec *codec;
//     AVCodecContext *c= NULL;
//     int out_size, len;
//     FILE *f, *outfile;
//     uint8_t *outbuf;
//     uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
//     AVPacket avpkt;

//     av_init_packet(&avpkt);

//     printf("Audio decoding\n");

//     /* find the mpeg audio decoder */
//     codec = avcodec_find_decoder(CODEC_ID_MP2);
//     if (!codec) {
//         fprintf(stderr, "codec not found\n");
//         exit(1);
//     }

//     c= avcodec_alloc_context();

//     /* open it */
//     if (avcodec_open(c, codec) < 0) {
//         fprintf(stderr, "could not open codec\n");
//         exit(1);
//     }

//     outbuf = malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);

//     f = fopen(filename, "rb");
//     if (!f) {
//         fprintf(stderr, "could not open %s\n", filename);
//         exit(1);
//     }
//     outfile = fopen(outfilename, "wb");
//     if (!outfile) {
//         av_free(c);
//         exit(1);
//     }

//     /* decode until eof */
//     avpkt.data = inbuf;
//     avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

//     while (avpkt.size > 0) {
//         out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//         len = avcodec_decode_audio3(c, (short *)outbuf, &out_size, &avpkt);
//         if (len < 0) {
//             fprintf(stderr, "Error while decoding\n");
//             exit(1);
//         }
//         if (out_size > 0) {
//             /* if a frame has been decoded, output it */
//             fwrite(outbuf, 1, out_size, outfile);
//         }
//         avpkt.size -= len;
//         avpkt.data += len;
//         if (avpkt.size < AUDIO_REFILL_THRESH) {
//             /* Refill the input buffer, to avoid trying to decode
//              * incomplete frames. Instead of this, one could also use
//              * a parser, or use a proper container format through
//              * libavformat. */
//             memmove(inbuf, avpkt.data, avpkt.size);
//             avpkt.data = inbuf;
//             len = fread(avpkt.data + avpkt.size, 1,
//                         AUDIO_INBUF_SIZE - avpkt.size, f);
//             if (len > 0)
//                 avpkt.size += len;
//         }
//     }

//     fclose(outfile);
//     fclose(f);
//     free(outbuf);

//     avcodec_close(c);
//     av_free(c);
}



int main(int argc, char **argv){
    const char *filename;

    /* must be called before using avcodec lib */

    // /* register all the codecs */
    av_register_all();

    if (argc <= 1) {
        audio_encode_example("test.mp3");
    //     audio_decode_example("/tmp/test.sw", "/tmp/test.mp2");
    //     filename = "/test.mpg";
    } else {
        filename = argv[1];
    }

    audio_decode_example("test.sw", filename);

    return 0;
}
