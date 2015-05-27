#include "Headers.h"

BaseGameObject* IrbagaWarrior::constructor()
{
	return new IrbagaWarrior();
}

void IrbagaWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = IRBAGA_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 10.0f;
	ActionRadius = 0.7f;
	GeometryRadius = 0.5f;
	Damage = 35;
	DamageTime = 1000.0f;
}