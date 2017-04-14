#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_
#include"nocopyable.h"
#include"Mutex.h"

#include<map>


using namespace std;

namespace http
{

class EventLoop;
class Acceptor;
class Channel;
class EventLoopThreadPool;
class ThreadPool;

class HttpServer:public nocopyable
{
	typedef std::map<int, Channel*> ConnectionMap;
private:
	int port_;
	char *ip_;
	Mutex mutex_;
	ConnectionMap connections_;
	EventLoop *mainloop_;
	
	EventLoopThreadPool *io_threadpool_;
	
	Acceptor * acceptor_;
	ThreadPool * calc_threadpool_;

	
public:
	ConnectionMap & getConnections() { return connections_; }
	HttpServer(int port,char *ip,int io_threadnum,unsigned int calc_threadnum);
	~HttpServer();
	void addChannel(Channel * cha);
	void delChannel(Channel * cha);
	void start();
	ThreadPool *  getCalcThreadPool(){ return calc_threadpool_; }; 
	EventLoop*  getNextLoop();

	
};	
	
}

#endif