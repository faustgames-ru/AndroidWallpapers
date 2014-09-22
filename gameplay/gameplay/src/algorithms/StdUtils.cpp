/*
 * StdUtils.cpp
 *
 *  Created on: Jan 29, 2014
 *      Author: Constantine
 */
#include "algorithms\StdUtils.h"
#include "Base.h"

CThreadLock::CThreadLock()
{
#ifdef __ANDROID__
    // init lock here
    pthread_mutex_init(&mutexlock, 0);
#endif
}

CThreadLock::~CThreadLock()
{
#ifdef __ANDROID__
    // deinit lock here
    pthread_mutex_destroy(&mutexlock);
#endif
}
void CThreadLock::Lock()
{
#ifdef __ANDROID__
    // lock
    pthread_mutex_lock(&mutexlock);
#endif
}
void CThreadLock::Unlock()
{
#ifdef __ANDROID__
    // unlock
    pthread_mutex_unlock(&mutexlock);
#endif
}

float sign(float value)
{
#ifdef __ANDROID__
	return 0;//-2.0f * signbit(value) + 1.0f;
#elif WIN32
    return (fabs(value) > 0.0000001f) ? value / fabs(value) : 0.0f;
#endif

}

const char* charsTostdString(char const* s)
{
    return s ? s : "";
}

const char* stdStringToChars(const std::string str)
{
    return str.empty() ? NULL : str.data();
}

