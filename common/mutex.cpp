#include "mutex.h"

namespace ACE {
    Mutex::Mutex(pthread_mutex_t &m): _m(m) {
        pthread_mutex_lock(&_m);
    }

    Mutex::~Mutex() {
        pthread_mutex_unlock(&_m);
    }

}// namespace ACE

/*
void func() {
    Mutex lock(mutex);
    todo();//// 出现异常时, lock 也能成功被析构
}

比下面安全
void func() {
    pthread_mutex_lock(&mutex);
    todo();
    pthread_mutex_unlock(&mutex);
}
*/
