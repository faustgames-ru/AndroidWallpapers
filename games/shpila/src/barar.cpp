#include "Headers.h"

BaseGameObject* BararWarrior::constructor()
{
	return new BararWarrior();
}

void BararWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = BARAR_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}