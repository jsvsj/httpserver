#include"EventLoop.h"
#include"Epoller.h"
#include"HttpServer.h"
#include"ThreadPool.h"
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

using namespace std;
using namespace http;



EventLoop::EventLoop(HttpServer* server)
	:server_(server),
	epoller_(new Epoller()),
	islooping_(false),
	initsize(16),
	actevents(initsize)
{
	
}

EventLoop::~EventLoop()
{
	delete epoller_;
}

void EventLoop::addEvent(Channel *cha)
{
	server_->addChannel(cha);
	epoller_->Epoll_Add(cha);
}

void EventLoop::delEvent(Channel *cha)
{
	epoller_->Epoll_Del(cha);
	server_->delChannel(cha);
}


void EventLoop::loop()
{
	islooping_=true;
	while(1)
	{
		cout<<"io thread ["<<pthread_self()<<" work]  epoll wait"<<endl;
		int num=epoller_->Epoll_Wait(actevents);
		cout<<"there are "<<num<<" events happen"<<endl;
		if(num==initsize)
		{
			initsize*=2;
			actevents.resize(initsize);
		}
		
		for(int i=0;i<num;++i)
		{
			((Channel *)actevents[i].data.ptr)->handleEvent();
		}
		
	}
}
	