#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class Player
{
public:
	GameObjectManager& Manager;
	Player* EnemyPlayer;
	bool AutoPlay;
	int ID;
	int UprgadeLevel;
	Vector3 BattleFieldDirection;
	int MainResource;
	Player(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection);
	void update(float time);
	void CreateWarrior(const char* name);
	int getNewObjectID();
	HiddenObject* getDefence();
private:
	CLink<HiddenObject> _defenceTower;
	CLink<HiddenObject> _defenceBase;
	float _spawnTimer;
	Vector3 _position;
	int _warriorsSpawnedCount;
	int _newObjectID;
	float _mainResourceIncreacetimer;
};

#endif