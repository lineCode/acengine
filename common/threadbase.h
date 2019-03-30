#ifndef __COMMON_THREADBASE_H__
#define __COMMON_THREADBASE_H__

class ThreadBase {
public:
	ThreadBase() {}
	virtual ~ThreadBase() {}
	virtual bool run() = 0;
	virtual void OnShutDown() {}
public:
	pthread_t _thread_handle;
};// class ThreadBase

#endif
