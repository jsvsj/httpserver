#ifndef _CONDITION_H_
#define _CONDITION_H_

#include"nocopyable.h"
#include<pthread.h>
#include"Mutex.h"

namespace http
{

class Condition:public nocopyable
{
private:
	Mutex &mutex_;
	pthread_cond_t pcond_;
public:
	explicit Condition(Mutex & mutex):mutex_(mutex)
	{
		pthread_cond_init(&pcond_,NULL);
	}
	
	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}
	
	bool waitForSeconds(int seconds);
	
	void wait()
	{
		pthread_cond_wait(&pcond_,mutex_.getPthreadMutex());
	}	
	
	void notify()
	{
		pthread_cond_signal(&pcond_);
	}
	
	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
	}
	
};

}



#endif
