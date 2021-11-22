CC=gcc
CFLAGS=-I./inc
PATH_SRC=./src
PATH_UT=./UnitTest
PATH_OBJ=./obj

histogram.o:
	$(CC) -c $(PATH_SRC)/histogram.c -o $(PATH_OBJ)/histogram.o $(CFLAGS)

UnitTest_histogram.o:
	$(CC) -c $(PATH_UT)/UnitTest_histogram.c -o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

UnitTest_histogram: histogram.o UnitTest_histogram.o
	$(CC) -o UnitTest_histogram.exe $(PATH_OBJ)/histogram.o $(PATH_OBJ)/UnitTest_histogram.o $(CFLAGS)

clean:
	rm $(PATH_OBJ)/*.o -f
