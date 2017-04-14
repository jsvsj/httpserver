#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include"Channel.h"
#include"EventLoop.h"

namespace http
{

class HttpServer;
class EventLoop;	
	
class Acceptor:public Channel
{
private:
	int port_;
	char *ip_;
	int index_;
	HttpServer* server_;
	EventLoop *loop_;
	
public:
	Acceptor(EventLoop *loop,HttpServer* server,int port,char *ip);
	~Acceptor();
	virtual void handleEvent();
	void Bind();
	void Listen();
	
	void handleRead();
	
};	
	
}

#endif