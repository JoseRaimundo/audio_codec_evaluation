
include Makefile.top


MODULE     = CodecTest

CFLAGS = -I$(INCDIR)/ \
        -I/usr/local/include/\
        -I/usr/include/\
	-Iinclude/\
        #-I../../common/xml/include/\
        #-I../common/include/\

CFLAGS += -g -ggdb -fPIC

## -lboost_iostreams 
###-lboost_system -lboost_filesystem

LDFLAGS = \
	-L/usr/local/lib\

SOURCES = \
	main.cpp


.SUFFIXES: .cpp .o .h

include Makefile.build

default: all