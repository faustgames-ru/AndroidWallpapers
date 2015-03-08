#ifndef HIDDENOBJECT_H_
#define HIDDENOBJECT_H_

#include "gameplay.h"
#include "BaseGameObject.h"
using namespace gameplay;

class HiddenObject : public BaseGameObject
{
public:
	Vector3 Position;
	CLink<BaseGameObject> Target;
	static BaseGameObject* constructor();
	HiddenObject();
	virtual const Vector3 position();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
};

#endif