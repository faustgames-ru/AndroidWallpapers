#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class Player
{
public:
	GameObjectManager& Manager;
	CLink<HiddenObject> Nexus;
	CLink<HiddenObject> EnemyNexus;
	bool AutoPlay;
	int ID;
	Player(GameObjectManager& manager, int id, Vector3 position);
	void update(float time);
	void CreateWarrior(const char* name);
	int getNewObjectID();
private:
	float _spawnTimer;
	Vector3 _position;
	int _warriorsSpawnedCount;
	int _newObjectID;
};

#endif