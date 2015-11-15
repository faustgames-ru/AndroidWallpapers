#include "Headers.h"

TimeWarpObject::TimeWarpObject()
: BaseGameObject()
{
}
BaseGameObject* TimeWarpObject::constructor()
{
	return new TimeWarpObject();
}
void TimeWarpObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseGameObject::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	_lifeTimer.enable(true);
	_lifeTimer.start(QBI_TIME_WARP_LIFE_TIME, 0.0f);
}
bool TimeWarpObject::interactive() 
{
	return true; 
}

void TimeWarpObject::interaction(BaseGameObject* object)
{
	BaseGameObject::interaction(object);
	if (!friendly(object) && checkDistanceToObject(object, QBI_TIME_WARP_RADIUS))
	{
		object->setAura(Aura::TimeWarpSlow, true);
	}
}

void TimeWarpObject::update(float time)
{
	BaseGameObject::update(time);
	if (_lifeTimer.action(time))
		_lifeTimer.enable(false);
}

bool TimeWarpObject::deleted()
{
	return !_lifeTimer.enabled();
}