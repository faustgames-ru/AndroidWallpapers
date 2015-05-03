#ifndef ALBIRIA_H_
#define ALBIRIA_H_

using namespace gameplay;

class AlbiriaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Vector3 position);
	virtual void update(float time);
};



#endif