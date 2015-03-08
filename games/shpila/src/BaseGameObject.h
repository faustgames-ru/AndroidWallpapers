#ifndef BASEGAMEOBJECT_H_
#define BASEGAMEOBJECT_H_

#include "gameplay.h"
#include "algorithms\Links.h"
using namespace gameplay;

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

	virtual const Vector3 position();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
protected:
	AutoRef<Node> _node;
	float getInteractionDistance(BaseGameObject* object);
	float _damageTimer;
};

#endif