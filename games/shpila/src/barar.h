#ifndef BARAR_H_
#define BARAR_H_

using namespace gameplay;

class BararWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_BARAR; }
};



#endif