#ifndef BASESTATICACTOR_H_
#define BASESTATICACTOR_H_

class BaseStaticActor : public BaseGameObject
{
public:
	BaseStaticActor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
	virtual void update(float time);
	virtual bool getDetected();
	virtual void setDetected(bool value);
	virtual void attackEnable(bool value);
protected:
	virtual void rangeFire();
	virtual void targeting(BaseGameObject* object);
	SimpleTimer _damageTimer;
	AnimatedFloat _fogOfWarTimer;
};

#endif