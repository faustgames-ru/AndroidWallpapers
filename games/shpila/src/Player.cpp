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
, _cell(0, 0)
, _mousePos()
{
	for (int i = 0; i < CellsCountX; i++)
	{
		for (int j = 0; j < CellsCountZ; j++)
		{
			Cells[i][j] = false;
			Cells[i][j] = false;
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
	_cornerPos = _centerPos - AxisX * _factor * 0.5f * (float)CellsCountX - AxisZ * _factor * 0.25f * (float)CellsCountZ;
}

void WarriorsGrid::mousOver(const Vector3 mousePos)
{
	_mousePos = mousePos;
	Vector3 dir = mousePos - _cornerPos;
	Vector3 localdir = Vector3(dir.dot(AxisX), 0.0, dir.dot(AxisZ));
	_mouseInGrid = (localdir.x >= 0.0f) && (localdir.x <= _factor * (float)CellsCountX) && (localdir.z >= 0.0f) && (localdir.z <= 0.5f * _factor * (float)CellsCountZ);
	_cell = worldToGrid(mousePos);
}

bool WarriorsGrid::isPlaceFree(int radius)
{
	bool res = true;
	switch (radius)//!!
	{
	case 3:
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ()));
	case 2:
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ()));
	case 1:
		res = res && !getCell(_cell);
	}
	return res;
}

const Vector3 WarriorsGrid::getPlacePosition(const int radius)
{
	Vector3 position = _cornerPos + AxisX * _factor * ((float)(_cell.PosZ % 2) * 0.5f + (float)_cell.PosX) + 0.5f * AxisZ * _factor * (float)_cell.PosZ;
	if (radius == 2)//!!
	{
		position = position - _factor * 0.5f * AxisX;
	}
	return position;
}

void WarriorsGrid::markUnitPlace(const int radius)
{
	GridCell cell(0, 0);
	switch (radius)//!!
	{
	case 3:
		setCell(worldToGrid(_mousePos + Vector3::unitX()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ()), true);
	case 2:
		setCell(worldToGrid(_mousePos - Vector3::unitX()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ()), true);
	case 1:
		setCell(_cell, true);
	}
	
}

void WarriorsGrid::render(const Matrix matrix, int radius)
{
	if (radius != 0)
	{
	_spriteBatch->start();
	_spriteBatch->setProjectionMatrix(matrix);
	
	//_spriteBatch->draw(_centerPos, AxisX, AxisZ, _factor * (float)CellsCountX, 0.5f * _factor * (float)CellsCountZ, 0, 0, 1, 1, Vector4(1, 1, 1, 1), Vector2::zero(), 0);
	
		Vector3 quadPos = _cornerPos;
		Vector4 color;
		for (int i = 0; i < CellsCountX; i++)
		{
			for (int j = 0; j < CellsCountZ; j++)
			{
				if ((j % 2) != 0)
					quadPos = _cornerPos + AxisX * _factor * (0.5f + (float)i) + 0.5f * AxisZ * _factor * (float)j;
				else
					quadPos = _cornerPos + AxisX * _factor * (float)i + 0.5f * AxisZ * _factor * (float)j;
				color = Cells[i][j] ? Vector4(0, 1, 0, 1) : Vector4(1, 1, 1, 0.25);
				_spriteBatch->draw(quadPos, Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
			}
		}
		if (_mouseInGrid)
		{
			switch (radius)//!!
			{
			case 3:
				color = getCell(worldToGrid(_mousePos + Vector3::unitX())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) + Vector3::unitX(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos + Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) + Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) + Vector3::unitX() + Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) - Vector3::unitX() + Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) + Vector3::unitX() - Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
			case 2:
				color = getCell(worldToGrid(_mousePos - Vector3::unitX())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) - Vector3::unitX(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos - Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) - Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
				color = getCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ())) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1) - Vector3::unitX() - Vector3::unitZ(), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
			case 1:
				color = getCell(_cell) ? Vector4(1, 0, 0, 1) : Vector4(0, 1, 0, 1);
				_spriteBatch->draw(getPlacePosition(1), Vector3::unitX(), Vector3::unitZ(), 0.9f, 0.9f, 0, 0, 1, 1, color, Vector2::zero(), 0);
			}
		}	
	_spriteBatch->finish();
	}
}

const GridCell WarriorsGrid::worldToGrid(const Vector3 point)
{
	GridCell res(0, 0);
	Vector3 dir = point - _cornerPos;
	Vector3 localdir = Vector3(dir.dot(AxisX), 0.0, dir.dot(AxisZ));
	res.PosZ = (localdir.z + 0.25f * _factor) / (0.5f *_factor);
	res.PosX = (localdir.x + 0.5f * _factor - (res.PosZ % 2) * 0.5f * _factor) / (_factor);
	return res;
}

bool WarriorsGrid::getCell(GridCell cell)
{
	return Cells[cell.PosX][cell.PosZ];
}

void WarriorsGrid::setCell(GridCell cell, bool value)
{
	Cells[cell.PosX][cell.PosZ] = value;
}

PlayerObject::PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection)
: Manager(manager)
, EnemyPlayer(NULL)
, AutoPlay(false)
, ID(id)
, UprgadeLevel(0)
, BattleFieldDirection(battleFieldDirection)
, BattleFieldMidPoint()
, UnitsOverMidLineCount(0)
, MainResource(0)
, _defenceTower()
, _defenceBase()
, _position(position)
, _warriorsSpawnedCount(0)
, _newObjectID(0)
, _mainResourceIncreaceTimer()
, _ExtractorBuildTimer()
, _gridGround()
, _gridAir()
, _CurrentCharacterName()
, _controlMid(false)
, _extractorsCount(0)
{
	_gridGround.AxisX = Vector3(BattleFieldDirection.z, BattleFieldDirection.y, -BattleFieldDirection.x);
	_gridGround.AxisZ = BattleFieldDirection;
	_gridGround.Position = _position;
	_gridGround.init();

	_gridAir.AxisX = Vector3(BattleFieldDirection.z, BattleFieldDirection.y, -BattleFieldDirection.x);
	_gridAir.AxisZ = BattleFieldDirection;
	_gridAir.Position = _position + Vector3(0.0f, AIR_UNITS_ALTITUDE, 0.0f);
	_gridAir.init();
	
	_mainResourceIncreaceTimer.start(1000.0f, 0.0f);
	_ExtractorBuildTimer.enable(false);
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
	if (_mainResourceIncreaceTimer.action(time))
		MainResource += 2;

	if (_ExtractorBuildTimer.action(time))
	{
		_extractorsCount++;
		_mainResourceIncreaceTimer.enable(true);
		_ExtractorBuildTimer.enable(false);
	}
		
	_controlMid = UnitsOverMidLineCount > EnemyPlayer->UnitsOverMidLineCount;
	UnitsOverMidLineCount = 0;
}

void PlayerObject::setCreateWarior(const char* name)
{
	_CurrentCharacterName = name;
}

bool PlayerObject::CreateWarrior(bool continuous, const Valuable<Vector3> position)
{
	if (_CurrentCharacterName.empty())
		return false;
	
	bool res = false;
	const ActorData& ad = getActorData(_CurrentCharacterName.c_str());
	if (ad.Price < MainResource)
	{
		WarriorsGrid &_grid = ad.MovementGround ? _gridGround : _gridAir;
		res = _grid.isPlaceFree(ad.CellsRadius());
		if (res)
		{
			const Vector3 warriorPosition = position.defined() ? position : _grid.getPlacePosition(ad.CellsRadius());
			_grid.markUnitPlace(ad.CellsRadius());
			BaseWarrior* warrior = (BaseWarrior*)Manager.createObject(_CurrentCharacterName.c_str(), warriorPosition, BattleFieldDirection, this);
			warrior->Holder = true;
			warrior->HolderWarriorName = _CurrentCharacterName.c_str();
			_warriorsSpawnedCount++;
			MainResource -= ad.Price;
			if (!continuous)
			{
				_CurrentCharacterName = "";
			}
				
		}
	}
	return res;
}

void PlayerObject::CancelCreateWarrior()
{
	_CurrentCharacterName = "";
}

void PlayerObject::addExtractor()
{
	if ((EXTRACTOR_PROCE < MainResource) && (_mainResourceIncreaceTimer.enabled()))
	{
		MainResource -= EXTRACTOR_PROCE;
		_ExtractorBuildTimer.start(EXTRACTOR_BUILD_TIME, 0.0f);
		_ExtractorBuildTimer.enable(true);
		_mainResourceIncreaceTimer.enable(false);
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

Vector3 PlayerObject::getPosition()
{
	return _position;
}

int PlayerObject::getAdditionalResourceIncreasePercent()
{
	int res = 0;
	if (_controlMid)
		res += 15;
	res += _extractorsCount * 10;
	return res;
}

int PlayerObject::getExtractorBuildingTime()
{
	return (int)(EXTRACTOR_BUILD_TIME - _ExtractorBuildTimer.getTime()) / 1000;
}

bool PlayerObject::isExtractorBuilding()
{
	return _ExtractorBuildTimer.enabled();
}

void PlayerObject::mousOver(const Vector3 mousePos)
{
	_gridGround.mousOver(mousePos);
	_gridAir.mousOver(mousePos);
}

void PlayerObject::render()
{
	const ActorData& ad = getActorData(_CurrentCharacterName.c_str());
	int radius = !_CurrentCharacterName.empty() ? ad.CellsRadius() : 0;	
	if (ad.MovementGround)
		_gridGround.render(Manager.scene()->getActiveCamera()->getViewProjectionMatrix(), radius);
	else
		_gridAir.render(Manager.scene()->getActiveCamera()->getViewProjectionMatrix(), radius);
}