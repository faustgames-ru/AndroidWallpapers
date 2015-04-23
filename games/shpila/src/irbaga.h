#ifndef IRBAGA_H_
#define IRBAGA_H_

using namespace gameplay;

class IrbagaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Vector3 position);
};



#endif