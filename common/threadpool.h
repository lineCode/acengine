/*--------------------------------------------
**  Module: threadpool 
**  Creator: Tango
**  Time: 2019-03-14
**  Function: threadpool
**------------------------------------------- */
#ifndef __COMMON_THREADPOOL_H__
#define __COMMON_THREADPOOL_H__
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <set>
#include "mutex.h"
#include "locker.h"
#include "threadbase.h"
using std::set;
namespace ACE {

int GenerateThreadId();

class ThreadController {
private:
	sem_t _sem;
	pthread_t _handle;
	int _thread_id;
public:
	void SetUp(pthread_t h) {
		_handle = h;
		sem_init(&_sem, PTHREAD_PROCESS_PRIVATE, 0);
		_thread_id = GenerateThreadId();
	}

	~ThreadController() {
		sem_destroy(&_sem);
	}

	// 挂起
	void Suspend() {
		// 等待信号量，如果信号量的值大于0， 将信号量的值减 1，如果信号量的值为0， 则阻塞
		sem_wait(&_sem);
	}

	// 唤醒
	void Resume() {
		// 释放信号量，使得信号量的值加1
		sem_post(&_sem);
	}

	void Join() {
		pthread_join(_handle, NULL);
	}

	uint32_t GetId() {
		return (uint32_t)_thread_id;
	}


};

struct Thread {
	ThreadBase *_execution_target;
	ThreadController _control_interface;
	Locker _setup_mutex;
	bool _delete_after_exit;
};

class ThreadPool {
public:
	ThreadPool();
	void IntergrityCheck();
	void StartUp();
	void ShutDown();
	bool ThreadExit(Thread *t);
	Thread* StartThread(ThreadBase *execution_target);
	void ExecuteTask(ThreadBase *extcution_target);
	void ShowStatus();
	void KillFreeThreads(uint32_t count);
	// ?
	void Gobble() {
		_threads_eaten = (int32_t)_free_threads.size();
	}
	uint32_t GetActiveThreadCount() {
		return (uint32_t)_actived_threads.size();
	}

	uint32_t GetFreeThreadCount() {
		return (uint32_t)_free_threads.size();
	}
	int GetNumCpus();
public:

	// 执行任务时会变
	int32_t _threads_requested_since_last_check;
	int32_t _threads_freed_since_last_check;
	int32_t _threads_exited_since_last_check;
	int32_t _threads_to_exit;
	// 执行任务时 -- 线程exit ++
	int32_t _threads_eaten;
	Locker _locker;
	std::set<Thread*> _actived_threads;
	std::set<Thread*> _free_threads;


};// class ThreadPool

extern ThreadPool g_thread_pool;
}// namespace ACE

#endif
