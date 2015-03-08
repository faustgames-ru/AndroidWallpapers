#include "BaseGameObject.h"

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

const Vector3 BaseGameObject::position()
{
	GP_ASSERT(_node);
	return _node->getTranslation();
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