#ifndef BASEWARRIOR_H_
#define BASEWARRIOR_H_

#include "Headers.h"

using namespace gameplay;

class BaseGameObject;

class BaseWarrior : public BaseGameObject
{
public:
	CLink<BaseGameObject> Target;
	CLink<BaseGameObject> EnemyNexus;
	BaseWarrior();
	static BaseGameObject* constructor();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
private:
	bool _initialized;
	bool _dead;
	AutoRef<Animation> _animation;
	AutoRef<AnimationClip> _clipCurrent;
	AutoRef<AnimationClip> _clipRun;
	AutoRef<AnimationClip> _clipAttack;
	AutoRef<AnimationClip> _clipDeath;
	AutoRef<AnimationClip> _clipDead;
	void updateAnimationState();
	void switchToAnimation(AnimationClip* clip, float repeatCount, unsigned long blendingTime);
};

#endif