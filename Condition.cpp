#include"Condition.h"
#include <time.h>
using namespace http;

bool Condition::waitForSeconds(int seconds)
{
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += seconds;
	return seconds == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}