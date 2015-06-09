#ifndef CHASOVOY_H_
#define CHASOVOY_H_

using namespace gameplay;

class ChasovoyWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_CHASOVOY; }
};

#endif