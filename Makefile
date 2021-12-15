CC=gcc
CFLAGS=-I./inc
PATH_SRC=./src
PATH_UT=./UnitTest
PATH_OBJ=./obj

SimpleCompressor.o:
	$(CC) -c $(PATH_SRC)/SimpleCompressor.c -o $(PATH_OBJ)/SimpleCompressor.o $(CFLAGS)

SimpleHistogram.o:
	$(CC) -c $(PATH_SRC)/SimpleHistogram.c -o $(PATH_OBJ)/SimpleHistogram.o $(CFLAGS)

histogram.o:
	$(CC) -c $(PATH_SRC)/histogram.c -o $(PATH_OBJ)/histogram.o $(CFLAGS)

FileManager.o:
	$(CC) -c $(PATH_SRC)/FileManager.c -o $(PATH_OBJ)/FileManager.o $(CFLAGS)

compressor.o:
	$(CC) -c $(PATH_SRC)/compressor.c -o $(PATH_OBJ)/compressor.o $(CFLAGS)

all: SimpleCompressor.o SimpleHistogram.o compressor.o histogram.o FileManager.o
	$(CC) -o SimpleCompressor.exe $(PATH_OBJ)/SimpleCompressor.o $(PATH_OBJ)/compressor.o $(PATH_OBJ)/histogram.o $(CFLAGS)
	$(CC) -o SimpleHistogram.exe $(PATH_OBJ)/SimpleHistogram.o $(PATH_OBJ)/histogram.o $(PATH_OBJ)/FileManager.o $(CFLAGS)

UnitTest_histogram.o:
	$(CC) -c $(PATH_UT)/UnitTest_histogram.c -o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

UnitTest_histogram:histogram.o UnitTest_histogram.o
	$(CC) -o UnitTest_histogram.exe $(PATH_OBJ)/histogram.o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

clean:
	rm $(PATH_OBJ)/*.o -f
