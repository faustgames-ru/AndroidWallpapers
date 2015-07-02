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
	ActionRadius = 5.0f;
	GeometryRadius = 0.5f;
	Damage = 6;
	DamageTime = 1000.0f * TIME_SCALE;
	Health = 80;
	Price = 125;
}