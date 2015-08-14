#include "Headers.h"

BaseGameObject* ChasovoyWarrior::constructor()
{
	return new ChasovoyWarrior();
}
void ChasovoyWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = CHASOVOY_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}