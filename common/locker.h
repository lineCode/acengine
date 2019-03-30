/*--------------------------------------------
**  Module: locker 
**  Creator: Tango
**  Time: 2019-03-14
**  Function: multi-thread locker
**------------------------------------------- */

#ifndef __COMMON_LOCKER_H__
#define __COMMON_LOCKER_H__

#include <pthread.h>
// 用于较长代码的加锁

class Locker {
public:
	Locker();
	virtual ~Locker();
	// 获取不到返回 false
	bool AttemptAcquire();
	void Acquire();
	void Release();
	
protected:
	// 多个锁使用一份
	static bool _s_attr_initalized;
	static pthread_mutexattr_t _s_attr;
	pthread_mutex_t _mutex;

};


#endif
