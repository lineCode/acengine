#include "locker.h"

bool Locker::_s_attr_initalized = false;
pthread_mutexattr_t Locker::_s_attr;

Locker::Locker() {
	if (!Locker::_s_attr_initalized) {
		pthread_mutexattr_init(&Locker::_s_attr);
		pthread_mutexattr_settype(&Locker::_s_attr, PTHREAD_MUTEX_RECURSIVE_NP);
		Locker::_s_attr_initalized = true;
	}

	pthread_mutex_init(&_mutex, &Locker::_s_attr);
}

bool Locker::AttemptAcquire() {
	return (0 == pthread_mutex_trylock(&_mutex));
}

void Locker::Acquire() {
	pthread_mutex_lock(&_mutex);
}

void Locker::Release() {
	pthread_mutex_lock(&_mutex);
}

Locker::~Locker() {
	pthread_mutex_destroy(&_mutex);
}

