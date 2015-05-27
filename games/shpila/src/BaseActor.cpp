#include "Headers.h"

BaseActor::BaseActor()
: BaseGameObject()
, _movementController()
{}

BaseActor::~BaseActor(){}

void BaseActor::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseGameObject::init(manager, node, playerID, transform);
	manager.registerMovementController(&_movementController);
	Vector3 pos;
	transform.getTranslation(&pos);
	_movementController.setPosition(OpenSteer::Vec3(pos.x, pos.y, pos.z));
}

void BaseActor::setPosition(const Vector3 pos)
{
	BaseGameObject::setPosition(pos);
	_movementController.setPosition(OpenSteer::Vec3(pos.x, pos.y, pos.z));
}