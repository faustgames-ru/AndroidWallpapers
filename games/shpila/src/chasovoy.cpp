#include "Headers.h"

BaseGameObject* ChasovoyWarrior::constructor()
{
	return new ChasovoyWarrior();
}
void ChasovoyWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = CHASOVOY_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.0f;
}