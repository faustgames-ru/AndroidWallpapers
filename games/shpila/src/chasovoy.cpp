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
	SearchRadius = 10.0f;
	ActionRadius = 2.5f;
	GeometryRadius = 0.5f;
	EnemyNexus = EnemyNexus;
	Damage = 35;
	DamageTime = 1000.0f;
}