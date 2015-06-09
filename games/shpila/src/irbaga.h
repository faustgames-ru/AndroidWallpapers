#ifndef IRBAGA_H_
#define IRBAGA_H_

using namespace gameplay;

class IrbagaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_IRBAGA; }
};



#endif