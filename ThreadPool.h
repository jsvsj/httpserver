#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include"Mutex.h"
#include"Thread.h"
#include"Condition.h"
#include<vector>
#include<deque>
using namespace std;

namespace http
{

class ThreadPool;

class Task:public nocopyable
{
	
public:
	Task()
	{
	}
	virtual ~Task()
	{
	}

    virtual int do_task() = 0;
};

class Thread_Task:public Thread
{
private:
	ThreadPool *pool_;
	bool autodel_;
public:
	Thread_Task(ThreadPool *pool,bool autodel):pool_(pool),autodel_(autodel)
	{
		
	}
	virtual void run();

};

class ThreadPool:public nocopyable
{
	
private:
	unsigned int threadsNum_;
	Condition condition_;
	Mutex mutex_;
	vector<Thread *> threads_;
	deque<Task *> queue_;
	
	
public:
	bool isrunning_;
	bool autodel_;
	ThreadPool(unsigned int threadsNum,bool autodel);
	~ThreadPool();
	void addTask(Task *task);
	void start();
	void stop();
	Task *take();
		
};

}

#endif