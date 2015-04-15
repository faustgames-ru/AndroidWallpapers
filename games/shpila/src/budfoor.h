#ifndef BUDFOOR_H_
#define BUDFOOR_H_

using namespace gameplay;

class BudfoorWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Vector3 position);
	virtual void update(float time);
};



#endif