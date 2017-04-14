#ifndef _MUTEX_H_
#define _MUTEX_H_
#include"nocopyable.h"
#include<pthread.h>
#include<cassert>

using namespace std;

namespace http
{

class Mutex:public nocopyable
{
	private:
		pthread_mutex_t mutex_;
		pthread_t holder_;
	public:
		Mutex():holder_(0)
		{
				int ret = pthread_mutex_init(&mutex_, NULL);
				assert(ret == 0);
		}
		
		~Mutex()
		{
			assert(holder_ == 0);
			int ret = pthread_mutex_destroy(&mutex_);
			assert(ret == 0); (void) ret;
		}

		void lock()
		{
			pthread_mutex_lock(&mutex_);
			holder_ = pthread_self();
		}
		void unlock()
		{
			holder_=0;
			pthread_mutex_unlock(&mutex_);
		}
		
		pthread_mutex_t* getPthreadMutex()
		{
			return &mutex_;
		}
};

class MutexManager:public nocopyable
{
	private:
		Mutex &mutex_;
	public:
		explicit MutexManager(Mutex & mutex):mutex_(mutex)
		{
			mutex_.lock();
		}
		~MutexManager()
		{
			mutex_.unlock();
		}
};


}


#endif