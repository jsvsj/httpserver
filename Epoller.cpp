#include"Epoller.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#define DEFAULT_SIZE 1024


using namespace std;
using namespace http;

Epoller::Epoller()
{
	epollfd_=epoll_create(DEFAULT_SIZE);
	if(epollfd_<0)
	{
		perror("epoll_create");
		exit(1);
	}
	
}


int Epoller::Epoll_Wait(ActiveEventList &actevents)
{
	int num=epoll_wait(epollfd_,&(*actevents.begin()),actevents.size(),-1);
	if(num<0)
	{
		perror("epoll_wait");
		exit(1);
	}
	for(int i=0;i<num;++i)
	{
		((Channel *)actevents[i].data.ptr)->setRevents(actevents[i].events);
	}
	return num;
	
}
void Epoller::Epoll_Del(Channel *cha)
{
	int fd=cha->getFd();
	int ret;
	cout<<"fd = "<<fd<<endl;
	ret=epoll_ctl(epollfd_,EPOLL_CTL_DEL,fd,NULL);
	if(ret<0)
	{
		cout<<"fd = "<<fd<<endl;
		perror("epoll_ctl del:");
		exit(1);
	}
	ret=close(fd);
	if(ret<0)
	{
		perror("close:");
	}
}

void Epoller::Epoll_Add(Channel *cha)
{
	struct epoll_event ev;
	int fd=cha->getFd();
	ev.events=cha->getEvents();
	
	ev.data.ptr=cha;
	
	int ret;
	ret=epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev);
	if(ret<0)
	{
		perror("epoll_ctl add:");
		//exit(1);
	}
}

Epoller::~Epoller()
{
	close(epollfd_);
}



