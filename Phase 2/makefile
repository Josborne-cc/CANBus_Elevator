CC = g++

CFLAGS = -Wall -std=c++11 
LFLAGS = -L/usr/lib -lmysqlcppconn -lboost_system
IFLAGS =  -I/usr/include/cppconn

default: run

run: main.o
	$(CC) $(CFLAGS) $(IFLAGS) -o run main.o $(LFLAGS)

run.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) *.o *~
