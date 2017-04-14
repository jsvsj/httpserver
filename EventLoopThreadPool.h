#ifndef _EVENT_LOOP_THREAD_POOL_
#define _EVENT_LOOP_THREAD_POOL_
#include"nocopyable.h"
#include"EventLoopThread.h"
#include<vector>

using namespace std;

namespace http
{

class EventLoop;
class HttpServer;
class EventLoopThread;

class EventLoopThreadPool:public nocopyable
{

private:
	HttpServer *server_;
	EventLoop *mainloop_;
	int threadnum_;
	int next_;
	vector<EventLoop *> loops_;
	vector<EventLoopThread *> threads_;
	
public:
	EventLoopThreadPool(HttpServer *server,EventLoop *loop,int threadnum);
	~EventLoopThreadPool();
	void start();
	EventLoop *getNextLoop();


};
	
}

#endif