#ifndef THEBASEOBJECT_H_
#define THEBASEOBJECT_H_

using namespace gameplay;

class BaseGameObject;

class TheBaseObject : public BaseStaticActor
{
public:
	CLink<BaseGameObject> Target;
	static BaseGameObject* constructor();
	TheBaseObject();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
};

#endif