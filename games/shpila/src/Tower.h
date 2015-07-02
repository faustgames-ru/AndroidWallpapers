#ifndef TOWEROBJECT_H_
#define TOWEROBJECT_H_

using namespace gameplay;

class BaseGameObject;

class TowerObject : public BaseGameObject
{
public:
	CLink<BaseGameObject> Target;
	static BaseGameObject* constructor();
	TowerObject();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
};

#endif