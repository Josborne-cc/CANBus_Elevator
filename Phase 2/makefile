CC = g++

CFLAGS = -Wall -std=c++11
LFLAGS = -L/usr/lib -lmysqlcppconn
IFLAGS =  -I/usr/include/cppconn

default: run

run: run.o
	$(CC) $(CFLAGS) -o run run.o

run.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
$(RM) run *.o *~
