#include"EventLoopThreadPool.h"
#include"HttpServer.h"
#include"EventLoop.h"
#include<iostream>


using namespace std;
using namespace http;


EventLoopThreadPool::EventLoopThreadPool(HttpServer *server,EventLoop *loop,int threadnum)
		:server_(server),
		mainloop_(loop),
		threadnum_(threadnum),
		next_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
	for(int i=0;i<threadnum_;++i)
	{
		delete threads_[i];	
	}
}


void EventLoopThreadPool::start()
{
	cout<<"io pool start"<<endl;
	for (int i = 0; i < threadnum_; ++i)
	{
		EventLoopThread* t = new EventLoopThread(server_);
	
		threads_.push_back(t);

		loops_.push_back(t->startLoop());
	}
  
}


EventLoop * EventLoopThreadPool::getNextLoop()
{
	EventLoop *loop;
	if(threadnum_==0)
	{
		loop=mainloop_;
	}
	else
	{
		loop = loops_[next_];
		next_=(next_+1)%threadnum_;
	}
	return loop;
	
}

