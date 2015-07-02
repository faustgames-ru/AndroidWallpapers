#include "Headers.h"

ActorData ACTOR_DATA[] =
{ 
	{ 
		"irbaga", 
		150.0f, //health
		90, //Price
		16.0f, //Damage
		1.5f, //Distance
		1100.0f * TIME_SCALE, //attack delay
	}, 
	{ 
		"budfoor", 
		120.0f, //health
		110, //Price
		10.0f, //Damage
		6.0f, //Distance
		1440.0f * TIME_SCALE, //attack delay
	}, 
	{ 
		"barar", 
		120.0f, //health
		275, //Price
		10.0f, //Damage
		4.0f, //Distance
		1440.0f * TIME_SCALE, //attack delay
	}, 
	{ 
		"albiria", 
		180.0f, //health
		150, //Price
		10.0f, //Damage
		5.0f, //Distance
		1100.0f * TIME_SCALE, //attack delay
	}, 
	{ 
		"chasovoy", 
		80.0f, //health
		125, //Price
		6.0f, //Damage
		5.0f, //Distance
		1000.0f * TIME_SCALE, //attack delay
	}, 
	{
		"core", 
		190.0f, //health
		200, //Price
		8.0f, //Damage
		5.0f, //Distance
		850.0f * TIME_SCALE, //attack delay
	} 


	,
	{
		"undefined",
		0.0f, //health
		0, //Price
		0.0f, //Damage
		0.0f, //Distance
		1000.0f * TIME_SCALE, //attack delay
	}
};

const ActorData& getActorData(const char* actorName)
{
	for (int i = 0; i < 6; i++)
	{
		if (!strcmp(actorName, ACTOR_DATA[i].Name.c_str()))
			return ACTOR_DATA[i];
	}
	return ACTOR_DATA[6];
}

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
	ActionRadius = 5.0f;
	GeometryRadius = 0.5f;
	Damage = 8;
	DamageTime = 850.0f * TIME_SCALE;
	_altitude = 3.75f;
	Health = 190;
	Price = 200;
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