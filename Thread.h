#ifndef _THREAD_H_
#define _THREAD_H_

#include"nocopyable.h"
#include<pthread.h>
#include<cassert>

namespace http
{
	
class Thread:public nocopyable
{
	
protected:
	pthread_t pid_;
	bool started_;
public:
	Thread();
	~Thread();
	void start();
	virtual void run()=0;
	int join();
	static void * startThread(void *thread);
	
};
	
}

#endif

		