#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

const float TIME_EPS = 0.00001f;

class TimerListener
{
public:
	virtual void handler() = 0;
	virtual bool enableHandler() = 0;
};

class BaseTimer
{
public:
	BaseTimer();
	~BaseTimer();
	virtual void update(float time);
	bool action(float time);
	void enable(bool isEnable);
protected:
	bool _ready;
	bool _enable;
private:
};

class SimpleTimer : public BaseTimer
{
public:
	SimpleTimer();
	void start(float time, float startTime);
	void update(float time);
private:
	float _time;
	float _timeCounter;
	float _startTime;
};

class AnimatedFloat : public BaseTimer
{
public:
	AnimatedFloat();
	void start(float vstart, float vend, float d, bool l, bool od) const;
	void startRandom(float vmin, float vmax, float d, bool l, bool od) const;
	void startToValue(float v);
	void setStaticValue(float v) const;
	void updateByValue(float v) const;
	virtual void update(float time);
	bool stoped();
	bool isEnable();
private:
	mutable float _value;
	mutable float _valuemin;
	mutable float _valuemax;
	mutable float _deltha;
	mutable bool _looped;
	mutable bool _oneDirectionLoop;
	mutable int _loopcounter;
	mutable float* _valueReference;
	float GetDeltha(float time);
	float GetRemaidAnimValue();
};

class TimeManager
{
public:
	TimeManager();
	~TimeManager();
	static TimeManager& instance();
	void add(BaseTimer * timer);
	void remove(BaseTimer * timer);
	void update(float time);
private:
	std::list<BaseTimer*> _timers;
};

#endif