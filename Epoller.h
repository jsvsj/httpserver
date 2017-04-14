#ifndef _EPOLL_H_
#define _EPOLL_H_
#include"nocopyable.h"
#include"Channel.h"

#include<vector>
#include<sys/epoll.h>

namespace http
{
	
class Channel;

class Epoller:public nocopyable
{
	
typedef std::vector<struct epoll_event> ActiveEventList;

private:
	int epollfd_;
	
public:
	Epoller();
	~Epoller();
	int Epoll_Wait(ActiveEventList &actevents);
	void Epoll_Del(Channel *cha);
	void Epoll_Add(Channel *cha);

};	

}



#endif