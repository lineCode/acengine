#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__
#include <pthread.h>
namespace ACE {

class Mutex
{
public:
    Mutex(pthread_mutex_t &m);
    ~Mutex();
private:
    Mutex(const Mutex &other);
    Mutex& operator = (const Mutex) {
		return *this;
	};
    // 这里需要是引用，确保锁的是同一个资源
    pthread_mutex_t &_m;
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_init.html
    // refs:So that portable code can be written that is invariant to this choice, 
    //  this volume of IEEE Std 1003.1-2001 does not define assignment or equality for this type, 
    //  and it uses the term "initialize" to reinforce the (more restrictive) notion that the lock 
    //      may actually reside in the mutex object itself.
};// CMutex

}// namespace ACE

#endif // __COMMON_MUTEX_H__

