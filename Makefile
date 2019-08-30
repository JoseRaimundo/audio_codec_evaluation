
include Makefile.top


MODULE     = CodecTest

CFLAGS = -I$(INCDIR)/ \
        -I/usr/local/include/\
        -I/usr/include/\
	-Iinclude/\
        # usr/local/src/ffmpeg/libswscale/libswscale.a \
        # /usr/local/src/ffmpeg/libavdevice/libavdevice.a \
        # /usr/local/src/ffmpeg/libavformat/libavformat.a \
        # /usr/local/src/ffmpeg/libavcodec/libavcodec.a \
        # /usr/local/src/ffmpeg/libavutil/libavutil.a \

CFLAGS += -g -ggdb -fPIC -lavformat -lavcodec -lavutil -lswscale -lpthread -lm -lz -lfaac -lmp3lame

## -lboost_iostreams 
###-lboost_system -lboost_filesystem

LDFLAGS = \
	-L/usr/local/lib\

SOURCES = \
	main.cpp


.SUFFIXES: .cpp .o .h

include Makefile.build

default: all