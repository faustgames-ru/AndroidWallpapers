#ifndef BASESTATICACTOR_H_
#define BASESTATICACTOR_H_

class BaseStaticActor : public BaseGameObject
{
public:
	BaseStaticActor();
protected:
	static void damageHandler(BaseGameObject* object);
	static bool damageEnableHandler(BaseGameObject* object);
	void rangeFire();
};

#endif