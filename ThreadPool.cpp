
#include"ThreadPool.h"
#include<iostream>

using namespace std;
using namespace http;


	
void Thread_Task::run()	
{
	while(pool_->isrunning_)
	{
		
		Task *task=pool_->take();
		if(task)
		{
			cout<<"["<<pthread_self()<<"]calc thread do task"<<endl;
			task->do_task();
			
		}
		if(autodel_)
		{
			delete task;
		}
	}
	
	
}
		

ThreadPool::ThreadPool(unsigned int threadsNum,bool autodel)
		:threadsNum_(threadsNum),
		condition_(mutex_),
		isrunning_(false),
		autodel_(autodel)
		
{
	threads_.reserve(threadsNum);
}

void ThreadPool::addTask(Task *task)
{
	cout<<"ThreadPool::addTask"<<endl;
	  if(threads_.empty())
	  {
		  task->do_task();
	  }
	  else
	  {
			MutexManager mmger(mutex_);
			queue_.push_back(task);
			condition_.notify();
	  }
}

void ThreadPool::start()
{
	isrunning_=true;
	cout<<"calc pool start"<<endl;
	for (int i = 0; i < threadsNum_; ++i)
	{
		threads_.push_back(new Thread_Task(this,true));
		threads_[i]->start();
	}

}

void ThreadPool::stop()
{
	cout<<"stop"<<endl;
	
	{
	  MutexManager mmger(mutex_);
	  isrunning_ = false;
	  condition_.notifyAll();
	}
	vector<Thread*>::iterator it;
	for(it=threads_.begin();it!=threads_.end();++it)
	{
		(*it)->join();
		delete *it;
	}
	
}

ThreadPool::~ThreadPool()
{
	if (isrunning_)
	{
		 
		stop();
	}
	cout<<"ThreadPool diet"<<endl;
	
	
}

Task * ThreadPool::take()
{
	Task *task=NULL;
	cout<<"ThreadPool::take"<<endl;
	MutexManager mmger(mutex_);
	while(queue_.empty() && isrunning_)
	{
		cout<<"calc ["<<pthread_self()<<  "]"<<"wait"<<endl;
		condition_.wait();
	}
	if(!queue_.empty())
	{
		task = queue_.front();
		queue_.pop_front();
	}
	return task;
}

