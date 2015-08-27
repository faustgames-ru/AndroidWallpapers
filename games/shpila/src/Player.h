#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class WarriorsGrid
{
public:
	static const int CellsCountX = 18;
	static const int CellsCountY = 6;

	WarriorsGrid();
	bool Cells[CellsCountX][CellsCountY];
	Vector3 AxisX;
	Vector3 AxisY;
};

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
	void CreateWarrior(const char* name, const Valuable<Vector3> position = Valuable<Vector3>::Undefined);
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