#include "Headers.h"

BaseGameObject* AlbiriaWarrior::constructor()
{
	return new AlbiriaWarrior();
}

void AlbiriaWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	float scale = 0.03f;
	BaseWarrior::init(manager, node, playerID, position);
	_node->setScale(scale, scale, scale);
	SearchRadius = 10.0f;
	ActionRadius = 3.5f;
	GeometryRadius = 0.5f;
	EnemyNexus = EnemyNexus;
	Damage = 35;
	DamageTime = 1000.0f;
}

void AlbiriaWarrior::update(float time)
{
	BaseWarrior::update(time);
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y + 2.5f, pos.z));
	Quaternion rot;
	Quaternion::multiply(_node->getRotation(), Quaternion(Vector3(0.0f, 1.0f, 0.0f), MATH_PI), &rot);
	_node->setRotation(rot);
}