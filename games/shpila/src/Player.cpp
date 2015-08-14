#include "Headers.h"

PlayerObject::PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection)
: Manager(manager)
, EnemyPlayer(NULL)
, AutoPlay(false)
, ID(id)
, UprgadeLevel(0)
, BattleFieldDirection(battleFieldDirection)
, MainResource(0)
, _defenceTower()
, _defenceBase()
, _position(position)
, _warriorsSpawnedCount(0)
, _newObjectID(0)
, _mainResourceIncreacetimer(0.0f)
{
}


void PlayerObject::update(float time)
{
	//create own nexus
	if (_defenceBase == NULL)
	{
		_defenceBase = (TheBaseObject*)Manager.createObject("base", _position + 25.45f * BattleFieldDirection, this);
		_defenceBase->SearchRadius = 10.0f;
	}

	if (_defenceTower == NULL)
	{
		_defenceTower = (TowerObject*)Manager.createObject("tower", _position + 45.25f * BattleFieldDirection, this);
		_defenceTower->SearchRadius = 10.0f;
	}
	//spawn warriors
	_mainResourceIncreacetimer += time;
	if (_mainResourceIncreacetimer > 1000.0f)
	{
		MainResource += 2;
		_mainResourceIncreacetimer = 0.0f;
	}
}

void PlayerObject::CreateWarrior(const char* name)
{
	const ActorData& ad = getActorData(name);
	//if (ad.Price < MainResource)
	{
		BaseWarrior* warrior = (BaseWarrior*)Manager.createObject(name, _position + Vector3(rnd(-10.0f, 10.0f), 0.0f, rnd(-10.0f, 10.0f)), this);
		warrior->Holder = true;
		warrior->HolderWarriorName = name;
		_warriorsSpawnedCount++;
		MainResource -= ad.Price;
	}
}

int PlayerObject::getNewObjectID()
{
	return _newObjectID++;
}

BaseStaticActor* PlayerObject::getDefence()
{
	return _defenceTower->LocalGameData.Health > 0.0 ? (BaseStaticActor*)_defenceTower : (BaseStaticActor*)_defenceBase;
}