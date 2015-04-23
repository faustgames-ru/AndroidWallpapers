#include "Headers.h"

BaseGameObject* BararWarrior::constructor()
{
	return new BararWarrior();
}

void BararWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	float scale = 0.01f;
	BaseWarrior::init(manager, node, playerID, position);
	_node->setScale(scale, scale, scale);
	SearchRadius = 10.0f;
	ActionRadius = 0.7f;
	GeometryRadius = 0.5f;
	EnemyNexus = EnemyNexus;
	Damage = 35;
	DamageTime = 1000.0f;
}