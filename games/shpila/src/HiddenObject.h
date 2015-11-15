#ifndef HIDDENOBJECT_H_
#define HIDDENOBJECT_H_

using namespace gameplay;

class BaseGameObject;

class HiddenObject : public BaseGameObject
{
public:
	Vector3 Position;
	CLink<BaseGameObject> Target;
	static BaseGameObject* constructor();
	HiddenObject();
	virtual const Vector3 position();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
};

#endif