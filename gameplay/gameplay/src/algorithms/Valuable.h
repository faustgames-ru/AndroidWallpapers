/*
 * VectorUtils.h
 *
 *  Created on: July 03, 2014
 *      Author: Constantine
 */

#ifndef VALUABLE_H_
#define VALUABLE_H_

template <typename T>
class Valuable
{
public:
	Valuable();
	Valuable(const Valuable& src);
	Valuable(const T& value, bool defined = true);
	~Valuable();

	bool defined();
	void reset();
	operator T&();
	T* operator ->();
	T& operator= (T value);

private:
	T _value;
	bool _defined;
};

template <typename T>
Valuable<T>::Valuable()
: _value()
, _defined(false)
{
}

template <typename T>
Valuable<T>::Valuable(const Valuable& src)
: _value(src._value)
, _defined(src._defined)
{
}

template <typename T>
Valuable<T>::Valuable(const T& value, bool defined)
: _value(value)
, _defined(defined)
{
}

template <typename T>
Valuable<T>::~Valuable()
{
}

template <typename T>
bool Valuable<T>::defined()
{
	return _defined;
}

template <typename T>
void Valuable<T>::reset()
{
	_defined = false;
}

template <typename T>
Valuable<T>::operator T&()
{
	return _value;
}

template <typename T>
T* Valuable<T>::operator->()
{
	return &_value;
}

template <typename T>
T& Valuable<T>::operator= (T value)
{
	_value = value;
	_defined = true;
	return _value;
}

#endif /* VALUABLE_H_ */
