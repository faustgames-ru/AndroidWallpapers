#include "Headers.h"

BaseGameObject::BaseGameObject()
: PlayerID(0)
, SearchRadius(0.0f)
, ActionRadius(0.0f)
, GeometryRadius(0.0f)
, Health(100.0f)
, Damage(0.0f)
, DamageTime(0.0f)
, _node()
, _damageTimer(0.0f)
, _movementController()
{}

void BaseGameObject::interaction(BaseGameObject* object)
{
	
}

void BaseGameObject::update(float time)
{
	GP_ASSERT(_node);
}

void BaseGameObject::setNode(Node* node)
{
	_node = node;
}

Node* BaseGameObject::node()
{
	return _node;
}

UnitMovementBase& BaseGameObject::MovementController()
{
	return _movementController;
}

void BaseGameObject::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	PlayerID = playerID;
	manager.registerObject(this);
	manager.registerMovementController(&_movementController);
	if (node)
	{
		Node* n = node->clone();
		setNode(n);
		setPosition(position);
		manager.registerSceneNode(n);
		//Vector3 forvard = _node->getForwardVectorWorld().normalize();
		//_movementController.setForward(forvard.x, forvard.y, forvard.z);
	}
}

const Vector3 BaseGameObject::position()
{
	GP_ASSERT(_node);
	return _node->getTranslation();
}

void BaseGameObject::setPosition(const Vector3 pos)
{
	if (_node)
	{
		_node->setTranslation(pos);
	}
	_movementController.setPosition(OpenSteer::Vec3(pos.x, pos.y, pos.z));
}

float BaseGameObject::getInteractionDistance(BaseGameObject* object)
{
	float xy = max(abs(object->position().x - position().x), abs(object->position().y - position().y));
	return max(xy, abs(object->position().z - position().z));
}

bool BaseGameObject::InteractionPossible(BaseGameObject* object)
{
	float radius = max(max(SearchRadius, GeometryRadius), ActionRadius);
	return radius > getInteractionDistance(object);
}