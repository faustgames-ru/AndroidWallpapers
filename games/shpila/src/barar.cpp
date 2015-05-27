#include "Headers.h"

BaseGameObject* BararWarrior::constructor()
{
	return new BararWarrior();
}

void BararWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = BARAR_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 10.0f;
	ActionRadius = 0.7f;
	GeometryRadius = 0.5f;
	EnemyNexus = EnemyNexus;
	Damage = 35;
	DamageTime = 1000.0f;
}