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
, SearchRadius(15.0f)
, LocalGameData()
, TypeName()
, UsedForIllusion(false)
, _manager(NULL)
, _node()
, _positionOnServer()
, _synkPositionMode(false)
{
}

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

bool BaseGameObject::illusionable()
{
	return false;
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

float BaseGameObject::getTargetingDistance(BaseGameObject* object)
{
	return object->getDetected() ? max(getDetectDistance(object), getAttackDistance(object)) : getDetectDistance(object);
}

bool BaseGameObject::checkDistanceToObject(BaseGameObject* object, float value)
{
	if (object == NULL)
		return 0.0f;
	else
		return Vector2(position().x, position().z).distanceSquared(Vector2(object->position().x, object->position().z)) < SQR(value + LocalGameData.GameData->GeometryRadius + object->LocalGameData.GameData->GeometryRadius);
}

bool BaseGameObject::checkTargetingDistanceToObject(BaseGameObject* object)
{
	if (object == NULL)
		return 0.0f;
	else
	{
		float value = (object->LocalGameData.GameData->MovementAir ? LocalGameData.GameData->DistanceAir : LocalGameData.GameData->DistanceGround);
		value = max(value, object->LocalGameData.GameData->DetectionDistance);
		return Vector2(position().x, position().z).distanceSquared(Vector2(object->position().x, object->position().z)) < SQR(value + LocalGameData.GameData->GeometryRadius + object->LocalGameData.GameData->GeometryRadius);
	}
}

bool BaseGameObject::friendly(BaseGameObject* object)
{
	return Player->ID == object->Player->ID;
}

void BaseGameObject::update(float time)
{
	for (int i = 0; i < (int)Aura::Last; i++)
	{
		setAura((Aura::Value)i, false);
	}
}

Node* BaseGameObject::node()
{
	return _node;
}

void BaseGameObject::attachNode(const char* nodeName, Node* node)
{
	if (nodeName == NULL)
	{
		_node->addChild(node);
	}
}

bool BaseGameObject::getAura(Aura::Value aura)
{
	return LocalGameData.getAura(aura);
}
void BaseGameObject::setAura(Aura::Value aura, bool enabled)
{
	LocalGameData.setAura(aura, enabled);
}

void BaseGameObject::updateMaterials(char* materialsFilename)
{
	Model* model = dynamic_cast<Model*>(_node->getDrawable());
	if (model)
	{
		for (int i = 0; i < (int)model->getMeshPartCount(); i++)
		{
			Material *mat = model->getMaterial(i);
			std::string materialPath = materialsFilename;
			materialPath.append("#");
			materialPath.append(mat->getName());
			Material* material = Material::create(materialPath.c_str());
			if (material)
			{
				int partIndex = model->getMesh()->getPartCount() > 0 ? i : -1;
				model->setMaterial(material, partIndex);
				SAFE_RELEASE(material);
			}
		}
	}
}

void BaseGameObject::setTransparency(float transparency)
{
	Model* model = dynamic_cast<Model*>(_node->getFirstChild()->getDrawable());
	if (model)
	{
		for (int i = 0; i < (int)model->getMeshPartCount(); i++)
		{
			Material *mat = model->getMaterial(i);
			mat->getParameter("u_modulateAlpha")->setFloat(transparency);
		}
	}
}

BaseGameObject* BaseGameObject::constructor()
{
	return new BaseGameObject();
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