#ifndef BASEWARRIOR_H_
#define BASEWARRIOR_H_

#include "Headers.h"

using namespace gameplay;

class BaseGameObject;

class UnitAnimation
{
public:
	enum Actions
	{
		Run,
		Attack,
		Death,
		Dead
	};
	typedef std::map<Actions, AutoRef<AnimationClip>> ActionsMap;

	AutoRef<Animation> _animation;
	AutoRef<AnimationClip> _clipCurrent;
	ActionsMap* _clips;

	UnitAnimation()
	: _clipCurrent()
	, _clips(new ActionsMap())
	{}

	UnitAnimation(ActionsMap* clips)
	: _clipCurrent()
	, _clips(clips)
	{}

	~UnitAnimation()
	{
		SAFE_DELETE(_clips);
	}
};

class BaseWarrior : public BaseGameObject
{
public:
	CLink<BaseGameObject> Target;
	CLink<BaseGameObject> EnemyNexus;
	BaseWarrior();
	~BaseWarrior();
	static BaseGameObject* constructor();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
private:
	bool _initialized;
	bool _dead;
	std::vector<UnitAnimation*> _unitAnimation;
	void updateAnimationState();
	void switchToAnimation(UnitAnimation::Actions action, float repeatCount, unsigned long blendingTime);
};

#endif