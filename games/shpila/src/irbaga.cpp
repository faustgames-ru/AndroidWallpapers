#include "Headers.h"

BaseGameObject* IrbagaWarrior::constructor()
{
	return new IrbagaWarrior();
}

void IrbagaWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = IRBAGA_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}