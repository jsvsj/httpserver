#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include<vector>
#include <sys/epoll.h>
#include"nocopyable.h"

namespace http
{

class Epoller;
class HttpServer;
class Channel;

class EventLoop:public nocopyable
{
	
	typedef std::vector<struct epoll_event> ActiveEventList;
private:
	HttpServer *server_;
	Epoller *epoller_;
	bool islooping_;
	int initsize;
	ActiveEventList actevents;
	
	
public:
	EventLoop(HttpServer *server);
	~EventLoop();
	void addEvent(Channel *cha);
	void delEvent(Channel *cha);
	void loop();
	
};
	
}

#endif