#include "Headers.h"

TimeWarpObject::TimeWarpObject()
: BaseGameObject()
{
}
BaseGameObject* TimeWarpObject::constructor()
{
	return new TimeWarpObject();
}
void TimeWarpObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseGameObject::init(manager, gameData, node, player, transform);
}
bool TimeWarpObject::interactive() 
{
	return true; 
}
void TimeWarpObject::update(float time)
{
	BaseGameObject::update(time);
}