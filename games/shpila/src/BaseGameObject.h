#ifndef BASEGAMEOBJECT_H_
#define BASEGAMEOBJECT_H_

using namespace gameplay;

class GameObjectManager;

class BaseGameObject : public CLinkObject
{
public:
	int PlayerID;
	int ID;
	float SearchRadius;
	float ActionRadius;
	float GeometryRadius;
	float Health;
	float Damage;
	float DamageTime;
	BaseGameObject();
	~BaseGameObject();
	bool InteractionPossible(BaseGameObject* object);
	Node* node();

	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return -1; }
	virtual const Vector3 position();
	virtual void setPosition(const Vector3 pos);
	virtual void setPositionOnServer(const Vector3 pos);
	virtual void interaction(BaseGameObject* object);
	virtual bool volumed();
	virtual bool interactive();
	virtual void update(float time);
	virtual bool deleted();
protected:
	GameObjectManager* _manager;
	AutoRef<Node> _node;
	float getInteractionDistance(BaseGameObject* object);
	float _damageTimer;
	Valuable<Vector3> _positionOnServer;
};

#endif