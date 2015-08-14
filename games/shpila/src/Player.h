#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class PlayerObject
{
public:
	GameObjectManager& Manager;
	PlayerObject* EnemyPlayer;
	bool AutoPlay;
	int ID;
	int UprgadeLevel;
	Vector3 BattleFieldDirection;
	int MainResource;
	PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection);
	void update(float time);
	void CreateWarrior(const char* name);
	int getNewObjectID();
	BaseStaticActor* getDefence();
private:
	CLink<TowerObject> _defenceTower;
	CLink<TheBaseObject> _defenceBase;
	Vector3 _position;
	int _warriorsSpawnedCount;
	int _newObjectID;
	float _mainResourceIncreacetimer;
};

#endif