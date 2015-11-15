#ifndef TOWEROBJECT_H_
#define TOWEROBJECT_H_

using namespace gameplay;

class BaseGameObject;

class TowerObject : public BaseStaticActor
{
public:
	static BaseGameObject* constructor();
	TowerObject();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
private:
	
};

#endif