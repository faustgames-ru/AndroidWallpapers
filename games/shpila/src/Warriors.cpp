#include "Headers.h"

ActorData ACTOR_DATA[] =
{ 
	{ 
		"irbaga", 
		150.0f, //health
		90, //Price
		16.0f, //Damage
		1.0f, //Distance
		1200.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	},
	{
		"chasovoy",
		80.0f, //health
		125, //Price
		6.0f, //Damage
		5.0f, //Distance
		1000.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	},
	{ 
		"budfoor", 
		160.0f, //health
		110, //Price
		10.0f, //Damage
		6.0f, //Distance
		1440.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	},
	{
		"dark",
		120.0f, //health
		175, //Price
		45.0f, //Damage
		1.0f, //Distance
		1690.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{ 
		"barar", 
		120.0f, //health
		275, //Price
		0.0f, //Damage
		4.0f, //Distance
		1440.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{
		"archon",
		360.0f, //health
		325, //Price
		25.0f, //Damage
		3.0f, //Distance
		1750.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{
		"observer",
		60.0f, //health
		125, //Price
		0.0f, //Damage
		4.0f, //Distance
		1440.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{
		"immortal",
		300.0f, //health
		255, //Price
		20.0f, //Damage
		6.0f, //Distance
		1450.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{
		"colossus",
		350.0f, //health
		460, //Price
		30.0f, //Damage
		6.0f, //Distance
		1650.0f * TIME_SCALE, //attack delay
		2, //UprgadeRequired
	},
	{
		"albiria",
		180.0f, //health
		150, //Price
		10.0f, //Damage
		5.0f, //Distance
		1100.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{ 
		"voidray", 
		250.0f, //health
		300, //Price
		6.0f, //Damage
		6.0f, //Distance
		500.0f * TIME_SCALE, //attack delay
		1, //UprgadeRequired
	},
	{
		"carrier",
		450.0f, //health
		550, //Price
		0.0f, //Damage
		8.0f, //Distance
		1100.0f * TIME_SCALE, //attack delay
		2, //UprgadeRequired
	},
	{
		"tempest",
		450.0f, //health
		475, //Price
		30.0f, //Damage
		15.0f, //Distance
		3300.0f * TIME_SCALE, //attack delay
		2, //UprgadeRequired
	},
	{
		"mothership",
		700.0f, //health
		700, //Price
		6.0f, //Damage
		7.0f, //Distance
		2210.0f * TIME_SCALE, //attack delay
		2, //UprgadeRequired
	},
	{
		"mothershipcore", 
		190.0f, //health
		200, //Price
		8.0f, //Damage
		5.0f, //Distance
		850.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	},
	{
		"tower",
		1000.0f, //health
		200, //Price
		25.0f, //Damage
		15.0f, //Distance
		500.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	},
	{
		"base",
		2000.0f, //health
		200, //Price
		25.0f, //Damage
		15.0f, //Distance
		500.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	}


	,
	{
		"undefined",
		0.0f, //health
		0, //Price
		0.0f, //Damage
		0.0f, //Distance
		1000.0f * TIME_SCALE, //attack delay
		0, //UprgadeRequired
	}
};

const ActorData& getActorData(const char* actorName)
{
	for (int i = 0; i < ACTOR_TYPE_LAST; i++)
	{
		if (!strcmp(actorName, ACTOR_DATA[i].Name.c_str()))
			return ACTOR_DATA[i];
	}
	return ACTOR_DATA[ACTOR_TYPE_LAST];
}

BaseGameObject* DarkWarrior::constructor()
{
	return new DarkWarrior();
}
DarkWarrior::DarkWarrior()
: BaseWarrior()
{}

void DarkWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 0.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ArchonWarrior::constructor()
{
	return new ArchonWarrior();
}
ArchonWarrior::ArchonWarrior()
: BaseWarrior()
{
}
void ArchonWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ObserverWarrior::constructor()
{
	return new ObserverWarrior();
}
ObserverWarrior::ObserverWarrior()
: BaseWarrior()
{
}
void ObserverWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ImmortalWarrior::constructor()
{
	return new ImmortalWarrior();
}
ImmortalWarrior::ImmortalWarrior()
: BaseWarrior()
{
}
void ImmortalWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ColossusWarrior::constructor()
{
	return new ColossusWarrior();
}
ColossusWarrior::ColossusWarrior()
: BaseWarrior()
{}
void ColossusWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* VoidRayWarrior::constructor()
{
	return new VoidRayWarrior();
}
VoidRayWarrior::VoidRayWarrior()
: BaseWarrior()
{}
void VoidRayWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* CarrierWarrior::constructor()
{
	return new CarrierWarrior();
}
CarrierWarrior::CarrierWarrior()
: BaseWarrior()
{}
void CarrierWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 2.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* TempestWarrior::constructor()
{
	return new TempestWarrior();
}
TempestWarrior::TempestWarrior()
: BaseWarrior()
{}
void TempestWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* MothershipWarrior::constructor()
{
	return new MothershipWarrior();
}

MothershipWarrior::MothershipWarrior()
: BaseWarrior()
{}

void MothershipWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 2.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* CoreWarrior::constructor()
{
	return new CoreWarrior();
}

CoreWarrior::CoreWarrior()
: BaseWarrior()
, _altitude(0.0f)
{
}

void CoreWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 2.0f;
	_altitude = 3.75f;
}

void CoreWarrior::update(float time)
{
	BaseWarrior::update(time);
	if (_dead)
	{
		_altitude = (_altitude > 0.0f) ? _altitude - 0.002f * time : 0.0f;
	}
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y + _altitude, pos.z));
}