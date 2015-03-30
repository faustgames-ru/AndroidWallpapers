#ifndef BASEGAMEOBJECT_H_
#define BASEGAMEOBJECT_H_

using namespace gameplay;

class GameObjectManager;

class BaseGameObject : public CLinkObject
{
public:
	int PlayerID;
	float SearchRadius;
	float ActionRadius;
	float GeometryRadius;
	float Health;
	float Damage;
	float DamageTime;
	BaseGameObject();
	bool InteractionPossible(BaseGameObject* object);
	void setNode(Node* node);
	Node* node();
	UnitMovementBase& MovementController();

	virtual void init(GameObjectManager& manager, Node* node, int playerID, Vector3 position);
	virtual const Vector3 position();
	virtual void setPosition(const Vector3 pos);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
protected:
	AutoRef<Node> _node;
	float getInteractionDistance(BaseGameObject* object);
	float _damageTimer;
	UnitMovementBase _movementController;
};

#endif