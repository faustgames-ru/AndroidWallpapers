#ifndef CHASOVOY_H_
#define CHASOVOY_H_

using namespace gameplay;

class ChasovoyWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

#endif