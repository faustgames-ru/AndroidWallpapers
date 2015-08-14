#ifndef IRBAGA_H_
#define IRBAGA_H_

using namespace gameplay;

class IrbagaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};



#endif