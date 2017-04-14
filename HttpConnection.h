#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_
#define BUF_SIZE 1024*10
#include<sys/socket.h>
#include<iostream>
#include<string>
#include"ThreadPool.h"


#include"Channel.h"

using namespace std;



namespace http
{

class HttpServer;
class EventLoop;



class HttpConnection:public Channel
{

private:
	HttpServer* server_;
	EventLoop *loop_;
	char recvbuf_[BUF_SIZE];
	int recvlen_;
	
	void recvdata();
	
public:
	HttpConnection(EventLoop *loop,HttpServer* server,int fd,int events);
	~HttpConnection();
	
	char * getRecvbuf(){ return recvbuf_; }
	EventLoop * getLoop(){return loop_; }
	int getRecvlen(){ return recvlen_; }
	
	virtual void handleEvent();
	
	virtual void handleRead();
	virtual void handleWrite();
	
};	
	
}

#endif