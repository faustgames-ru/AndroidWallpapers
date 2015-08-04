#include "Headers.h"

BaseGameObject::BaseGameObject()
: PlayerID(0)
, ID(0)
, Holder(false)
, GameData(NULL)
, SearchRadius(0.0f)
, Health(100.0f)
, _manager(NULL)
, _node()
, _damageTimer(0.0f)
, _positionOnServer()
, _synkPositionMode(false)
{}

BaseGameObject::~BaseGameObject()
{
	if (_node)
	{
		_manager->scene()->removeNode(_node);
	}
}

void BaseGameObject::interaction(BaseGameObject* object)
{
	
}

bool BaseGameObject::volumed()
{
	return true;
}

bool BaseGameObject::interactive()
{
	return true;
}

bool BaseGameObject::deleted()
{
	return false;
}

void BaseGameObject::update(float time)
{
	GP_ASSERT(_node);
}

Node* BaseGameObject::node()
{
	return _node;
}

void BaseGameObject::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	_manager = &manager;
	PlayerID = playerID;
	manager.registerObject(this);
	if (node)
	{
		_node.newRef(node->clone());
		_node->set(transform);
		manager.registerSceneNode(_node);
	}
	Vector3 translation;
	transform.getTranslation(&translation);
	setPosition(translation);

	Health = GameData->HP;
}

int BaseGameObject::ActorType() 
{ 
	return GameData->ActorType; 
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
}

void BaseGameObject::setPositionOnServer(const Vector3 pos)
{
	_positionOnServer = pos;
}

float BaseGameObject::getInteractionDistance(BaseGameObject* object)
{
	float xy = max(abs(object->position().x - position().x), abs(object->position().y - position().y));
	return max(xy, abs(object->position().z - position().z));
}

bool BaseGameObject::InteractionPossible(BaseGameObject* object)
{
	float radius = max(max(SearchRadius, GameData->GeometryRadius), GameData->DistanceGround);
	return radius > getInteractionDistance(object);
}