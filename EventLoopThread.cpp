#include"EventLoopThread.h"
#include"HttpServer.h"
#include"EventLoop.h"
#include<stdio.h>

using namespace http;


EventLoopThread::EventLoopThread(HttpServer *server)
	:mutex_(),
	condition_(mutex_),
	server_(server),
	loop_(NULL)
	
{
	
}

EventLoop* EventLoopThread::startLoop()
{
	this->start();
	MutexManager mmg(mutex_);
	while(loop_==NULL)
	{
		condition_.wait();
	}
	return loop_;
	
}

void EventLoopThread::run()
{
	{
		MutexManager mmg(mutex_);
		loop_=new EventLoop(server_);
		condition_.notify();
	}

	
	loop_->loop();
	
}

EventLoopThread::~EventLoopThread()
{
	delete loop_;
}






