#include <unistd.h>
#include <iostream>
#include "threadpool.h"
#include "logger.h"
using std::cout;
using std::endl;

#define THREAD_RESERVE 10

namespace ACE {
volatile int threadid_count = 0;
int GenerateThreadId() {
	int i = ++threadid_count;
	return i;
}

ThreadPool::ThreadPool() {
	_threads_requested_since_last_check = 0;
	_threads_freed_since_last_check = 0;
	_threads_exited_since_last_check = 0;
	_threads_to_exit = 0;
	_threads_eaten = 0;
}

static void *thread_proc(void *param) {
	Thread *t = (Thread*)param;
	if (!t)	return NULL;

	t->_setup_mutex.Acquire();
	cout<<"ThreadPoll"<<" Thread %u started."<<t->_control_interface.GetId();
	t->_setup_mutex.Release();

	for (;;) {
		if (t->_execution_target != NULL) {
			if (t->_execution_target->run()) {
				delete t->_execution_target;
			}

			t->_execution_target = NULL;
		}

		if (!g_thread_pool.ThreadExit(t)) {
			break;
		}
		else {
			t->_control_interface.Suspend();
		} 
		
	}

	pthread_exit(0);
}

Thread *ThreadPool::StartThread(ThreadBase *execution_target) {
	pthread_t target;
	Thread *t = new Thread;
	t->_execution_target = execution_target;
	t->_delete_after_exit = false;

	_locker.Acquire();

	t->_setup_mutex.Acquire();
	pthread_create(&target, NULL, &thread_proc, (void*)t);
	t->_control_interface.SetUp(target);
	// 子线程结束，资源自动回收
	pthread_detach(target);
	t->_setup_mutex.Release();

	_locker.Release();
	return t;
}

void ThreadPool::StartUp() {

	int tcount = THREAD_RESERVE;
	for (int i = 0; i < tcount; ++i) {
		StartThread(NULL);
	}

	LogInfo("threadpool", "startup %d threads\n", tcount);
}

void ThreadPool::ShowStatus() {
	_locker.Acquire();
	LogInfo("ThreadPool", "======");
	_locker.Release();
}

void ThreadPool::IntergrityCheck() {
	_locker.Acquire();
	int32_t gobbled = _threads_eaten;
	if (gobbled < 0) {
		uint32_t new_threads = abs(gobbled) + THREAD_RESERVE;
		for (uint32_t i = 0; i < new_threads; ++i) {
			StartThread(NULL);
		}	
	}
	else if (gobbled < THREAD_RESERVE) {
		uint32_t new_threads = (THREAD_RESERVE - gobbled);
		for (uint32_t i = 0; i < new_threads; ++i) {
			StartThread(NULL);
		}	
	}
	else if (gobbled > THREAD_RESERVE) {
		uint32_t kill_count = (gobbled - THREAD_RESERVE);
		// kill 线程只是挂起
		KillFreeThreads(kill_count);
		_threads_eaten -= kill_count;	
	}
	else {
		LogInfo("threadpoll", "perfect");
	}
	
	_threads_requested_since_last_check = 0;
	_threads_freed_since_last_check = 0;
	_threads_exited_since_last_check = 0;

	_locker.Release();
}

bool ThreadPool::ThreadExit(Thread *t) {
	if (!t) return false;

	_locker.Acquire();
	_actived_threads.erase(t);

	if (_threads_to_exit > 0) {
		--_threads_to_exit;
		++_threads_exited_since_last_check;
		if (t->_delete_after_exit) {
			_free_threads.erase(t);	
		}

		_locker.Release();
		delete t;
		return false;
	}

	++_threads_exited_since_last_check;
	++_threads_eaten;
	auto itr = _free_threads.find(t);
	if (itr != _free_threads.end())  {
		cout<<"error"<<endl;
	}

	_free_threads.insert(t);
	_locker.Release();
	return true;
}

void ThreadPool::KillFreeThreads(uint32_t count) {
	_locker.Acquire();
	Thread *t;
	auto itr = _free_threads.begin();
	for (; itr != _free_threads.end(); ++itr) {
		t = *itr;
		t->_execution_target = NULL;
		t->_delete_after_exit = true;
		++_threads_to_exit;
		t->_control_interface.Resume();
	}

	_locker.Release();
}

void ThreadPool::ShutDown() {
	_locker.Acquire();
	size_t tcount = _actived_threads.size() + _free_threads.size();
	// ThreadExit 才从 _free_threads 从删除
	// KillFreeThreads 只是计数和标记
	KillFreeThreads((uint32_t)_free_threads.size());
	_threads_to_exit += (uint32_t)_actived_threads.size();

	for (auto itr = _actived_threads.begin(); itr != _actived_threads.end(); ++itr) {
		Thread *t = *itr;
		if (t->_execution_target) {
			t->_execution_target->OnShutDown();
		}
		else {
			t->_control_interface.Resume();
		}
	}
	_locker.Release();
	
	for (int i = 0; ; ++i) {
		_locker.Acquire();
		if (_free_threads.size() || _actived_threads.size()) {
			if (i != 0 || _free_threads.size()) {
				Thread *t;
				auto itr = _free_threads.begin();
				for (; itr != _free_threads.end(); ++itr) {
					t = *itr;
					t->_control_interface.Resume();
				}
			}	
			
			_locker.Release();
			sleep(1000);
			continue;
		}

		_locker.Release();
	}
}



void ThreadPool::ExecuteTask(ThreadBase *execution_target) {
	Thread *t;
	_locker.Acquire();

	++_threads_requested_since_last_check;
	--_threads_eaten;

	if (_free_threads.size()) {
		t = * _free_threads.begin();
		_free_threads.erase(_free_threads.begin());
		t->_execution_target = execution_target;
		// 唤醒
		t->_control_interface.Resume();
	}
	else {
		t = StartThread(execution_target);		
	}

	_actived_threads.insert(t);
	_locker.Release();
}

ThreadPool g_thread_pool;

}// namespace ACE
