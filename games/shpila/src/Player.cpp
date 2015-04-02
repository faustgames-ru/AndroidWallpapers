#include "Headers.h"

Player::Player(GameObjectManager& manager, int id, Vector3 position)
: Manager(manager)
, Nexus()
, EnemyNexus()
, AutoPlay(false)
, ID(id)
, _spawnTimer(0.0f)
, _position(position)
, _warriorsSpawnedCount(0)
{
}


void Player::update(float time)
{
	//create own nexus
	if (Nexus == NULL)
	{
		Nexus = (HiddenObject*)Manager.createObject("tower", _position, ID);
		Nexus->Position = _position;
		Nexus->SearchRadius = 3.0f;
		Nexus->ActionRadius = 0.7f;
		Nexus->GeometryRadius = 0.5f;
		Nexus->Damage = 50.0f;
		Nexus->DamageTime = 1000.0f;
	}
	//find enemy one
	if (EnemyNexus == NULL)
	{
		for (std::vector<Player*>::iterator it = Manager.Players.begin(); it != Manager.Players.end(); ++it)
		{
			if ((*it)->ID != ID)
			{
				EnemyNexus = (*it)->Nexus;
				break;
			}
		}
	}
	//spawn warriors
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
	BaseWarrior* zealot = (BaseWarrior*)Manager.createObject(name, _position + Vector3(rnd(-1.0f, 1.0f), 0.0f, rnd(-3.0f, 3.0f)), ID);
	zealot->SearchRadius = 10.0f;
	zealot->ActionRadius = 0.7f;
	zealot->GeometryRadius = 0.5f;
	zealot->EnemyNexus = EnemyNexus;
	zealot->Damage = 35;
	zealot->DamageTime = 1000.0f;
	_warriorsSpawnedCount++;
}