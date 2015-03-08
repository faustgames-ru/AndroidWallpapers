#ifndef PLAYER_H_
#define PLAYER_H_

#include "gameplay.h"
#include "GameObjectManager.h"
#include "algorithms\Utils.h"
#include "BaseWarrior.h"
#include "HiddenObject.h"

using namespace gameplay;

class GameObjectManager;

class Player
{
public:
	GameObjectManager& Manager;
	CLink<HiddenObject> Nexus;
	CLink<HiddenObject> EnemyNexus;
	int ID;
	Player(GameObjectManager& manager, int id, Vector3 position);
	void update(float time);
private:
	float _spawnTimer;
	Vector3 _position;
	int _warriorsSpawnedCount;
};

#endif