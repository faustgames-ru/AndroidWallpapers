#ifndef THEBASEOBJECT_H_
#define THEBASEOBJECT_H_

using namespace gameplay;

class BaseGameObject;

class TheBaseObject : public BaseGameObject
{
public:
	CLink<BaseGameObject> Target;
	static BaseGameObject* constructor();
	TheBaseObject();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
};

#endif