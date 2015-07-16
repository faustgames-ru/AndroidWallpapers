#include "Headers.h"

Player::Player(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection)
: Manager(manager)
, EnemyPlayer(NULL)
, AutoPlay(false)
, ID(id)
, UprgadeLevel(0)
, BattleFieldDirection(battleFieldDirection)
, MainResource(0)
, _defenceTower()
, _defenceBase()
, _spawnTimer(0.0f)
, _position(position)
, _warriorsSpawnedCount(0)
, _newObjectID(0)
, _mainResourceIncreacetimer(0.0f)
{
}


void Player::update(float time)
{
	//create own nexus
	if (_defenceBase == NULL)
	{
		_defenceBase = (HiddenObject*)Manager.createObject("base", _position + 25.45f * BattleFieldDirection, ID);
		_defenceBase->SearchRadius = 10.0f;
		_defenceBase->GeometryRadius = 0.5f;
	}

	if (_defenceTower == NULL)
	{
		_defenceTower = (HiddenObject*)Manager.createObject("tower", _position + 45.25f * BattleFieldDirection, ID);
		_defenceTower->SearchRadius = 10.0f;
		_defenceTower->GeometryRadius = 0.5f;
	}
	//spawn warriors
	_mainResourceIncreacetimer += time;
	if (_mainResourceIncreacetimer > 1000.0f)
	{
		MainResource += 2;
		_mainResourceIncreacetimer = 0.0f;
	}
	_spawnTimer += time;
	if (AutoPlay)
	{
		if (_spawnTimer >= 3000.0f)
		{
			//if (_warriorsSpawnedCount < 100)
			{
				CreateWarrior("zealot");
				_spawnTimer = 0.0f;
			}
		}
	}
}

void Player::CreateWarrior(const char* name)
{
	const ActorData& ad = getActorData(name);
	//if (ad.Price < MainResource)
	{
		BaseWarrior* warrior = (BaseWarrior*)Manager.createObject(name, _position + Vector3(rnd(-10.0f, 10.0f), 0.0f, rnd(-10.0f, 10.0f)), ID);
		warrior->Player = this;
		warrior->Holder = true;
		warrior->HolderWarriorName = name;
		_warriorsSpawnedCount++;
		MainResource -= ad.Price;
	}
}

int Player::getNewObjectID()
{
	return _newObjectID++;
}

HiddenObject* Player::getDefence()
{
	return _defenceTower->Health > 0.0 ? _defenceTower : _defenceBase;
}