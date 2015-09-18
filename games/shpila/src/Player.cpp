#include "Headers.h"

#define SPRITE_VSH "res/shaders/sprite.vert"
#define SPRITE_FSH "res/shaders/sprite.frag"

WarriorsGrid::WarriorsGrid()
: AxisX()
, AxisZ()
, _spriteBatch(NULL)
, _factor(1.0f)
, _centerPos()
, _cornerPos()
, _mouseInGrid(false)
, _gridPosX(0)
, _gridPosZ(0)
{
	for (int i = 0; i < CellsCountX; i++)
	{
		for (int j = 0; j < CellsCountY; j++)
		{
			Cells[i][j][0] = false;
			Cells[i][j][1] = false;
		}
	}
}

WarriorsGrid::~WarriorsGrid()
{
	SAFE_DELETE(_spriteBatch);
}

void WarriorsGrid::init()
{
	_spriteBatch = SpriteBatch::create("res\\png\\warriorsgrid.png");
	_spriteBatch->getStateBlock()->setDepthWrite(false);
	_spriteBatch->getStateBlock()->setDepthTest(true);

	_factor = sqrt(2.0f);
	_centerPos = Position + Vector3(0.0f, 0.1f, 0.0f);
	_cornerPos = _centerPos - AxisX * _factor * 0.5f * (float)CellsCountX - AxisZ * _factor * 0.5f * (float)CellsCountY;
}

void WarriorsGrid::mousOver(const Vector3 mousePos)
{
	Vector3 dir = mousePos - _cornerPos;
	Vector3 localdir = Vector3(dir.dot(AxisX), 0.0, dir.dot(AxisZ));
	_mouseInGrid = (localdir.x >= 0.0f) && (localdir.x <= _factor * (float)CellsCountX) && (localdir.z >= 0.0f) && (localdir.z <= _factor * (float)CellsCountY);
	_gridPosX = (0.5f + localdir.x) / _factor;
	_gridPosZ = (0.5f + localdir.z) / _factor;
	int halfX = (int)((0.5f + localdir.x) / (0.5f * _factor));
	int halfZ = (int)((0.5f + localdir.z) / (0.5f * _factor));
	_gridPosX_ = ((halfX % 2) != 0) && ((halfZ % 2) != 0) ? 1 : 0;
}

bool WarriorsGrid::isPlaceFree()
{
	return !Cells[_gridPosX][_gridPosZ][_gridPosX_];
}

const Vector3 WarriorsGrid::getPlacePosition()
{
	Vector3 position = _cornerPos + AxisX * _factor * (float)_gridPosX + AxisZ * _factor * (float)_gridPosZ;
	if (_gridPosX_ > 0)
		position = position + _factor * 0.5f * AxisX + _factor * 0.5f * AxisZ;
	return position;
}

void WarriorsGrid::markUnitPlace()
{
	Cells[_gridPosX][_gridPosZ][_gridPosX_] = true;
}

void WarriorsGrid::render(const Matrix matrix)
{
	_spriteBatch->start();
	_spriteBatch->setProjectionMatrix(matrix);
	_spriteBatch->draw(_centerPos, AxisX, AxisZ, _factor * (float)CellsCountX, _factor * (float)CellsCountY, 0, 0, 1, 1, Vector4(1, 1, 1, 1), Vector2::zero(), 0);	
	Vector3 quadPos = _cornerPos;
	Vector4 color;
	for (int i = 0; i < CellsCountX; i++)
	{
		for (int j = 0; j < CellsCountY; j++)
		{
			quadPos = _cornerPos + AxisX * _factor * (float)i + AxisZ * _factor * (float)j;
			color = Cells[i][j][0] ? Vector4(1, 0, 0, 1) : Vector4(1, 1, 1, 1);
			_spriteBatch->draw(quadPos, Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
			quadPos = quadPos + _factor * 0.5f * AxisX + _factor * 0.5f * AxisZ;
			color = Cells[i][j][1] ? Vector4(1, 0, 0, 1) : Vector4(1, 1, 1, 1);
			_spriteBatch->draw(quadPos, Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
		}
	}
	if (_mouseInGrid)
	{
		_spriteBatch->draw(getPlacePosition(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, Vector4(1, 0, 0, 1), Vector2::zero(), 0);
	}	
	_spriteBatch->finish();
}

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
, _grid()
{
	_grid.AxisX = Vector3(BattleFieldDirection.z, BattleFieldDirection.y, -BattleFieldDirection.x);
	_grid.AxisZ = BattleFieldDirection;
	_grid.Position = _position;
	_grid.init();
}


void PlayerObject::update(float time)
{
	//create own nexus
	if (_defenceBase == NULL)
	{
		_defenceBase = (TheBaseObject*)Manager.createObject("base", _position + 25.45f * BattleFieldDirection, BattleFieldDirection, this);
		_defenceBase->SearchRadius = 20.0f;
	}

	if (_defenceTower == NULL)
	{
		_defenceTower = (TowerObject*)Manager.createObject("tower", _position + 45.25f * BattleFieldDirection, BattleFieldDirection, this);
		_defenceTower->SearchRadius = 20.0f;
	}
	//spawn warriors
	_mainResourceIncreacetimer += time;
	if (_mainResourceIncreacetimer > 1000.0f)
	{
		MainResource += 2;
		_mainResourceIncreacetimer = 0.0f;
	}
}

bool PlayerObject::CreateWarrior(const char* name, const Valuable<Vector3> position)
{
	bool res = false;
	const ActorData& ad = getActorData(name); 
	//if (ad.Price < MainResource)
	{
		res = _grid.isPlaceFree();
		if (res)
		{
			//const Vector3 warriorPosition = position.defined() ? position : _position + Vector3(rnd(-10.0f, 10.0f), 0.0f, rnd(-10.0f, 10.0f));
			const Vector3 warriorPosition = position.defined() ? position : _grid.getPlacePosition();
			_grid.markUnitPlace();
			BaseWarrior* warrior = (BaseWarrior*)Manager.createObject(name, warriorPosition, BattleFieldDirection, this);
			warrior->Holder = true;
			warrior->HolderWarriorName = name;
			_warriorsSpawnedCount++;
			MainResource -= ad.Price;
		}
	}
	return res;
}

int PlayerObject::getNewObjectID()
{
	return _newObjectID++;
}

BaseStaticActor* PlayerObject::getDefence()
{
	return _defenceTower->LocalGameData.Health > 0.0 ? (BaseStaticActor*)_defenceTower : (BaseStaticActor*)_defenceBase;
}

void PlayerObject::mousOver(const Vector3 mousePos)
{
	_grid.mousOver(mousePos);
}

void PlayerObject::render()
{
	_grid.render(Manager.scene()->getActiveCamera()->getViewProjectionMatrix());
}