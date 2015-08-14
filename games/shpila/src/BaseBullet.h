#ifndef BASEBULLET_H_
#define BASEBULLET_H_

using namespace gameplay;

class BaseGameObject;

class BaseBullet : public BaseGameObject
{
public:
	BaseBullet();
	~BaseBullet();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual bool interactive() { return false; }
	virtual void update(float time);
	virtual bool deleted();
protected:
	bool _deleted;
};

#endif