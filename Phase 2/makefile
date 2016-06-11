CC = g++

CFLAGS = -Wall -std=c++11 
LFLAGS = -L/usr/lib -lmysqlcppconn -lboost_system -lboost_thread
IFLAGS =  -I/usr/include/cppconn

default: run

run: AsyncSerial.o BufferedAsyncSerial.o  main.o
	$(CC) $(CFLAGS) $(IFLAGS) -o run  AsyncSerial.o BufferedAsyncSerial.o main.o $(LFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

BufferedAsyncSerial.o: BufferedAsyncSerial.cpp BufferedAsyncSerial.h
	$(CC) $(CFLAGS) -c BufferedAsyncSerial.cpp

AsyncSerial.o: AsyncSerial.cpp AsyncSerial.h
	$(CC) $(CFLAGS) -c AsyncSerial.cpp

clean:
	$(RM) *.o *~
