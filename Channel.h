#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include"nocopyable.h"

namespace http
{
	
class Channel
{
protected:	
	int fd_;
	int events_;
	int revents_;
public:
	Channel();
	virtual ~Channel();
	virtual void handleEvent()=0;
	int getEvents();
	int getRevents();
	int getFd();
	void setRevents(int events);
	void setEvents(int events);
	
	void foxonwrite();
	void foxonread();
	void clearfoxon();
	
	
};	
	
	
}

#endif

