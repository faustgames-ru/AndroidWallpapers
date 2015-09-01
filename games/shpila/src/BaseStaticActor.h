#ifndef BASESTATICACTOR_H_
#define BASESTATICACTOR_H_

class BaseStaticActor : public BaseGameObject
{
public:
	BaseStaticActor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void update(float time);
protected:
	virtual void rangeFire();
	SimpleTimer _damageTimer;
};

#endif