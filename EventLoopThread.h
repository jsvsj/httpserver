#ifndef _EVENT_LOOP_THREAD_
#define _EVENT_LOOP_THREAD_
#include"Thread.h"
#include"Mutex.h"
#include"Condition.h"

namespace http
{

class HttpServer;	
class EventLoop;

class EventLoopThread:public Thread
{
	
private:	
	Mutex mutex_;
	Condition condition_;
	EventLoop *loop_;
	HttpServer *server_;

public:
	EventLoopThread(HttpServer *server);
	~EventLoopThread();
	virtual void run();
	EventLoop* startLoop();
	
};	
	
	
}

#endif