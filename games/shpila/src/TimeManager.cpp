#include "Headers.h"
#include "algorithms\StdUtils.h"

BaseTimer::BaseTimer()
: _ready(false)
, _enable(true)
{
}

BaseTimer::~BaseTimer()
{
}

void BaseTimer::update(float time)
{

}

bool BaseTimer::action(float time)
{
	update(time);
	bool ready = _ready;
	_ready = false;
	return ready;
}

void BaseTimer::enable(bool isEnable)
{
	_enable = isEnable;
}

SimpleTimer::SimpleTimer()
: BaseTimer()
, _time(0.0f)
, _timeCounter(0.0f)
, _startTime(0.0f)
{
}

void SimpleTimer::start(float time, float startTime)
{
	_time = time;
	_timeCounter = startTime;
	_startTime = startTime;
}

void SimpleTimer::update(float time)//, BaseGameObject* object
{
	if (_enable || (_timeCounter < _startTime))
		_timeCounter += time;

	if (!_enable && (_timeCounter > _startTime))
		_timeCounter = _startTime;

	if (_enable)
	{
		if (_timeCounter > _time)
		{
			int nDiv = _timeCounter / _time;
			_timeCounter -= _time * (float)nDiv;
			_ready = true;
		}
	}
}

//AnimatedFloat

AnimatedFloat::AnimatedFloat()
: BaseTimer()
, _value(0.0)
, _valuemin(0.0)
, _valuemax(0.0f)
, _deltha(0.0)
, _looped(false)
, _oneDirectionLoop(false)
, _loopcounter(0)
, _valueReference(NULL)
{
}

void AnimatedFloat::start(float vstart, float vend, float d, bool l, bool od) const
{
	_looped = l;
	_oneDirectionLoop = od;
	_deltha = d;
	_loopcounter = 0;
	_value = _valuemin = vstart;
	_valuemax = vend;
}

void AnimatedFloat::startRandom(float vmin, float vmax, float d, bool l, bool od) const
{
	_loopcounter = 0;
	_value = rnd(vmin, vmax);
	_valuemin = vmin;
	_valuemax = vmax;
	_deltha = d;
	_looped = l;
	_oneDirectionLoop = od;
}
void AnimatedFloat::startToValue(float v)
{
	_loopcounter = 0;
	_deltha = (v > _value) ? fabs(_deltha) : _deltha = -fabs(_deltha);
	_valuemin = _value;
	_valuemax = v;
}

void AnimatedFloat::updateByValue(float v) const
{
	_valuemin += v;
	_valuemax += v;
	_value += v;
}

void AnimatedFloat::setStaticValue(float v) const
{
	_loopcounter = 0;
	_valuemin = _valuemax = _value = v;
	_deltha = 0.0f;
	_looped = false;

}
void AnimatedFloat::update(float time)
{
	if (isEnable())
	{
		_value += GetDeltha(time);
		bool outofrange = (_deltha > 0.0f) ? _value >= _valuemax : _value <= _valuemax;
		if (outofrange)
		{
			_value = _valuemax;
			if (_looped)
			{
				_loopcounter++;
				_value = (_oneDirectionLoop) ? _valuemin : _valuemax;
				_deltha = (_oneDirectionLoop) ? _deltha : -_deltha;
				if (!_oneDirectionLoop)
				{
					_valuemax = _valuemin;
					_valuemin = _value;
				}
			}
		}
	}
	if (_valueReference)
	{
		*_valueReference = _value;
	}
}

bool AnimatedFloat::stoped()
{
	return (!isEnable()) || (!_looped && (_value == _valuemax));
}

bool AnimatedFloat::isEnable()
{
	return (fabs(_deltha) > TIME_EPS) && _enable;
}

float AnimatedFloat::GetDeltha(float time)
{
	return _deltha * time;
}

float AnimatedFloat::GetRemaidAnimValue()
{
	return _valuemax - _value;
}