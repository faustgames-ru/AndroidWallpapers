#ifndef BARAR_H_
#define BARAR_H_

using namespace gameplay;

class BararWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
};



#endif