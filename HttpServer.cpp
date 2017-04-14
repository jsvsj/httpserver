#include"HttpServer.h"
#include"EventLoop.h"
#include"Acceptor.h"
#include"EventLoopThreadPool.h"
#include"ThreadPool.h"
#include<iostream>
#include<pthread.h>
#include<signal.h>

using namespace std;
using namespace http;


HttpServer::HttpServer(int port,char *ip,int io_threadnum,unsigned int calc_threadnum)
		:port_(port),
		ip_(ip),
		mutex_()
{
	mainloop_=new EventLoop(this);

	io_threadpool_=new EventLoopThreadPool(this,mainloop_,io_threadnum);
	
	acceptor_=new Acceptor(mainloop_,this,port_,ip_);

	if(calc_threadnum==0)
	{
		calc_threadpool_=NULL;
	}
	else
	{
		calc_threadpool_=new ThreadPool(calc_threadnum,true);
	}
	
	
}


void HttpServer::start()
{
	signal(SIGPIPE,SIG_IGN);
	acceptor_->Bind();
	acceptor_->Listen();
	
	cout<<"main thread is ["<<pthread_self()<<"]"<<endl;
	mainloop_->addEvent(acceptor_);

	if(calc_threadpool_!=NULL)
	{
		calc_threadpool_->start();
	}
	
	io_threadpool_->start();
	
	mainloop_->loop();
	
	
	
}


void HttpServer::addChannel(Channel * cha)
{
	MutexManager mmg(mutex_);
	int fd=cha->getFd();
	ConnectionMap::iterator it=connections_.find(fd);
	if(it==connections_.end())
	{
		connections_[fd]=cha;
	}
	else
	{
		delete it->second;
		connections_[fd]=cha;
	}
	cout<<"connections_.size:"<<connections_.size()<<endl;
}

void HttpServer::delChannel(Channel * cha)
{
	MutexManager mmg(mutex_);
	int fd=cha->getFd();
	ConnectionMap::iterator it=connections_.find(fd);
	if(it!=connections_.end())
	{
		delete it->second;
		connections_.erase(it);
	}
	cout<<"connections_.size:"<<connections_.size()<<endl;
}

EventLoop* HttpServer::getNextLoop()
{
	return io_threadpool_->getNextLoop();
}

HttpServer::~HttpServer()
{
	delete mainloop_;

	delete io_threadpool_;
	
	delete acceptor_;
	delete calc_threadpool_;
	
}



