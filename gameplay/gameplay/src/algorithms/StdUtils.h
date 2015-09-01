/*
 * VectorUtils.h
 *
 *  Created on: Jan 27, 2014
 *      Author: Constantine
 */
#ifndef STDUTILS_H_
#define STDUTILS_H_

#include "Base.h"
#ifdef __ANDROID__
#include <pthread.h>
#endif

class CThreadLock
{
public:
    CThreadLock();
    virtual ~CThreadLock();

    void Lock();
    void Unlock();
private:
#ifdef __ANDROID__
    pthread_mutex_t mutexlock;
#endif
};

template<class T>
void DeleteVectorPointers(std::vector<T> vect)
{
	for (int i = 0; i < (int)vect.size(); i++)
	{
		delete vect[i];
	}
}

template<class T>
void DeleteListPointers(std::list<T> list)
{
	for (std::list<T>::iterator it = list.begin(); it != list.end(); it++)
	{
		delete *it;
	}
}

template<typename S, typename T>
void DeleteMapPointers(std::map<S, T> m)
{
	//for(typename std::map<char, U>::const_iterator it = values.begin(); it != values.end(); it++)
	for (typename std::map<S, T>::iterator it = m.begin(); it != m.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
}

float sign(float value);

const char* charsTostdString(char const* s);
const char* stdStringToChars(const std::string str);


#endif /* STDUTILS_H_ */
