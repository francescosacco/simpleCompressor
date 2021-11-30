CC=gcc
CFLAGS=-I./inc
PATH_SRC=./src
PATH_UT=./UnitTest
PATH_OBJ=./obj

main.o:
	$(CC) -c $(PATH_SRC)/main.c -o $(PATH_OBJ)/main.o $(CFLAGS)

histogram.o:
	$(CC) -c $(PATH_SRC)/histogram.c -o $(PATH_OBJ)/histogram.o $(CFLAGS)

compressor.o:
	$(CC) -c $(PATH_SRC)/compressor.c -o $(PATH_OBJ)/compressor.o $(CFLAGS)

all: main.o compressor.o histogram.o
	$(CC) -o SimpleCompressor.exe $(PATH_OBJ)/main.o $(PATH_OBJ)/compressor.o $(PATH_OBJ)/histogram.o $(CFLAGS)

UnitTest_histogram.o:
	$(CC) -c $(PATH_UT)/UnitTest_histogram.c -o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

UnitTest_histogram:histogram.o UnitTest_histogram.o
	$(CC) -o UnitTest_histogram.exe $(PATH_OBJ)/histogram.o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

clean:
	rm $(PATH_OBJ)/*.o -f
