#include "Headers.h"

BaseGameObject* IrbagaWarrior::constructor()
{
	return new IrbagaWarrior();
}

void IrbagaWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	BaseWarrior::init(manager, node, playerID, position);
	SearchRadius = 10.0f;
	ActionRadius = 0.7f;
	GeometryRadius = 0.5f;
	Damage = 35;
	DamageTime = 1000.0f;
}