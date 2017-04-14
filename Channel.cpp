#include"Channel.h"
#include<unistd.h>
#include<sys/epoll.h>


using namespace http;

Channel::Channel()
	:events_(0),revents_(0),fd_(-1)
{
	
}

void Channel::setRevents(int revents)
{
	revents_=revents;
}

void Channel::setEvents(int events)
{
	events_=events;
}
	
void Channel::foxonwrite()
{
	events_=EPOLLOUT;
}

void Channel::foxonread()
{
	events_=EPOLLIN;
}

void Channel::clearfoxon()
{
	events_=0;
}

int Channel::getEvents()
{
	return events_;
}

int Channel::getRevents()
{
	return revents_;
}

int Channel::getFd()
{
	return fd_;
}

Channel::~Channel()
{
	close(fd_);
}


