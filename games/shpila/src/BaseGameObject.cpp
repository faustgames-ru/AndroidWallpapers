#include "Headers.h"

Timer::Timer(float time, float startTime, TimerEventHandler handler, TimerEventEnableHandler enableHandler)
: _time(time)
, _handler(handler)
, EnableHandler(enableHandler)
, _timeCounter(startTime)
, _startTime(startTime)
{
}

void Timer::update(float time, BaseGameObject* object)
{
	bool enabled = (EnableHandler != NULL) ? EnableHandler(object): true;

	if (enabled || (_timeCounter < _startTime))
		_timeCounter += time;

	if (!enabled && (_timeCounter > _startTime))
		_timeCounter = _startTime;

	if (enabled)
	{
		if (_timeCounter > _time)
		{
			_handler(object);
			_timeCounter = 0.0f;
		}
	}
}

BaseGameObject::BaseGameObject()
: Player(NULL)
, Target()
, ID(0)
, Holder(false)
, SearchRadius(0.0f)
, LocalGameData()
, _manager(NULL)
, _node()
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

bool BaseGameObject::getDetected()
{
	return false;
}
void BaseGameObject::setDetected(bool value)
{

}

bool BaseGameObject::deleted()
{
	return false;
}

void BaseGameObject::doDamage(BaseGameObject* object)
{
	if (object != NULL)
	{
		int attacksCount = LocalGameData.GameData->getAttacksCount(*object->LocalGameData.GameData);
		for (int i = 0; i < attacksCount; i++)
		{
			if (LocalGameData.GameData->ImmediateAttack)
			{
				LocalGameData.doDamage(object->LocalGameData);
			}
			else
			{
				rangeFire();
			}
		}
	}
}

bool BaseGameObject::isAttackToTargetAllowed(BaseGameObject* object)
{
	return object == NULL ? false : LocalGameData.isAttackToTargetAllowed(object->LocalGameData);
}

void BaseGameObject::rangeFire()
{

}

float BaseGameObject::getAttackDistance(BaseGameObject* object)
{
	if (object == NULL)
		return 0.0f;
	else
		return (object->LocalGameData.GameData->MovementAir ? LocalGameData.GameData->DistanceAir : LocalGameData.GameData->DistanceGround) +
			object->LocalGameData.GameData->GeometryRadius + LocalGameData.GameData->GeometryRadius;
}

float BaseGameObject::getDetectDistance(BaseGameObject* object)
{
	if (object == NULL)
		return 0.0f;
	else
		return object->LocalGameData.GameData->DetectionDistance + object->LocalGameData.GameData->GeometryRadius + LocalGameData.GameData->GeometryRadius;
}

void BaseGameObject::update(float time)
{	

}

Node* BaseGameObject::node()
{
	return _node;
}

void BaseGameObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	_manager = &manager;
	Player = player;
	LocalGameData.init(gameData);
	manager.registerObject(this);
	if (node)
	{
		_node.newRef(node->clone());
		_node->setOrientationAxises(Node::PositiveX, Node::PositiveZ, Node::PositiveY);
		_node->set(transform);
		manager.registerSceneNode(_node);
	}
	Vector3 translation;
	transform.getTranslation(&translation);
	setPosition(translation);
	//_FogOfWarTimer = createTimer(Timer(2.0f, 0.0f, ))
}

int BaseGameObject::ActorType() 
{ 
	return LocalGameData.GameData->ActorType; 
}

const Vector3 BaseGameObject::position()
{
	GP_ASSERT(_node);
	return _node->getTranslation();
}

const Vector3 BaseGameObject::massCenterPosition()
{
	Vector3 res = _node->getTranslation();
	res += Vector3(0.0f, LocalGameData.GameData->GeometryRadius, 0.0f);
	return res;
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
	float radius = max(max(SearchRadius, LocalGameData.GameData->GeometryRadius), LocalGameData.GameData->DistanceGround);
	return radius > getInteractionDistance(object);
}