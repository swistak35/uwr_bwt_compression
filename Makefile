CPPFLAGS=-Wall -Wextra -std=c++11
# DEBUGFLAGS=-g
DEBUGFLAGS=-O3
# LDFLAGS=-g $(shell root-config --ldflags)
# LDLIBS=$(shell root-config --libs)

SRCS=LexiBWT.cpp DeBWT.cpp Huffman.cpp MoveToFront.cpp DemoveToFront.cpp SuffixBWT.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

%.o: %.cpp %.h
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -c $< 

suffix: suffix.cpp SuffixBWT.o LexiBWT.o
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o suffix suffix.cpp SuffixBWT.o LexiBWT.o

uni: uni.cpp UniBWT.o LexiBWT.o
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o uni uni.cpp UniBWT.o LexiBWT.o

COMPRESS_LEXI_SRCS=LexiBWT.cpp Huffman.cpp MoveToFront.cpp
COMPRESS_LEXI_OBJS=$(subst .cpp,.o,$(COMPRESS_LEXI_SRCS))
compress-lexi: compress-lexi.cpp $(COMPRESS_LEXI_OBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o compress-lexi compress-lexi.cpp $(COMPRESS_LEXI_OBJS)

COMPRESS_UNI_SRCS=UniBWT.cpp Huffman.cpp MoveToFront.cpp
COMPRESS_UNI_OBJS=$(subst .cpp,.o,$(COMPRESS_UNI_SRCS))
compress-uni: compress-uni.cpp $(COMPRESS_UNI_OBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o compress-uni compress-uni.cpp $(COMPRESS_UNI_OBJS)

COMPRESS_SUFFIX_SRCS=SuffixBWT.cpp SuffixTree.cpp Huffman.cpp MoveToFront.cpp StackTreeMemory.cpp
COMPRESS_SUFFIX_OBJS=$(subst .cpp,.o,$(COMPRESS_SUFFIX_SRCS))
compress-suffix: compress-suffix.cpp $(COMPRESS_SUFFIX_OBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o compress-suffix compress-suffix.cpp $(COMPRESS_SUFFIX_OBJS)

DECOMPRESS0_SRCS=DeBWT.cpp Huffman.cpp DemoveToFront.cpp
DECOMPRESS0_OBJS=$(subst .cpp,.o,$(DECOMPRESS0_SRCS))
decompress0: decompress0.cpp $(DECOMPRESS0_OBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o decompress0 decompress0.cpp $(DECOMPRESS0_OBJS)

DECOMPRESS1_SRCS=DeBWT.cpp Huffman.cpp DemoveToFront.cpp
DECOMPRESS1_OBJS=$(subst .cpp,.o,$(DECOMPRESS1_SRCS))
decompress1: decompress1.cpp $(DECOMPRESS1_OBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -o decompress1 decompress1.cpp $(DECOMPRESS1_OBJS)

test: testing
	rm -f minipantadeusz.bin > /dev/null
	rm -f minipantadeusz2.txt > /dev/null
	time (./testing > /dev/null)
	diff minipantadeusz.txt minipantadeusz2.txt

clean:
	rm -f *.o
	rm -f tests/*.compressed
	rm -f tests/*.decompressed
	rm -f compress-lexi compress-suffix compress-uni decompress0 decompress1

all: compress-lexi compress-uni compress-suffix decompress0 decompress1
