#ifndef WARRIORS_H_
#define WARRIORS_H_

using namespace gameplay;

class ActorData
{
public:
	std::string Name;
	float Health;
	int Price;
	float Damage;
	float Distance;
	float AttackDelay;
};

extern ActorData ACTOR_DATA[];

const ActorData& getActorData(const char* actorName);

class CoreWarrior : public BaseWarrior
{
public:
	
	static BaseGameObject* constructor();
	CoreWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_CORE; }
	virtual void update(float time);
private:
	float _altitude;
};



#endif