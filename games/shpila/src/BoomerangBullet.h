#ifndef BOOMERANGBULLET_H_
#define BOOMERANGBULLET_H_

using namespace gameplay;

class BaseGameObject;

class BoomerangBullet : public BaseBullet
{
public:
	static BaseGameObject* constructor();
	virtual void update(float time);
};

#endif