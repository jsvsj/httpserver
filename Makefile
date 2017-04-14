.PHONY:clean
CC=g++
CFLAGS=-g -Wall
BIN=main
OBJS=main.o HttpServer.o HttpConnection.o Channel.o EventLoop.o Epoller.o \
Acceptor.o EventLoopThread.o Thread.o EventLoopThreadPool.o  \
ThreadPool.o Condition.o

LIBS=-lpthread
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
clean:
	rm -f *.o $(BIN)
