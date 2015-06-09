#ifndef BUDFOOR_H_
#define BUDFOOR_H_

using namespace gameplay;

class BudfoorWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_BUDFOOR; }
	virtual void update(float time);
};

#endif