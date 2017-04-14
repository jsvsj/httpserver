#include"Thread.h"
#include<iostream>
#include<stdio.h>

using namespace http;
	
Thread::Thread():pid_(0),started_(false)
{
	
}
	
void Thread::start()
{
	started_=true;
	int ret=pthread_create(&pid_,NULL,startThread,this);
	if(ret!=0)
	{
		printf( " This fake error is in %s on line %d\n ",__FILE__, __LINE__);
	}
}

int Thread::join()
{
	assert(started_);
	return pthread_join(pid_, NULL);
}

void* Thread::startThread(void* thread)
{
	Thread * pThread = static_cast<Thread *>(thread);
	pThread->run();
	
}

Thread::~Thread()
{
	
}

		