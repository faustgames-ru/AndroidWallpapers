#ifndef BASEBULLET_H_
#define BASEBULLET_H_

using namespace gameplay;

class BaseGameObject;

class BaseBullet : public BaseGameObject
{
public:
	CLink<BaseGameObject> Target;
	BaseBullet();
	~BaseBullet();
	virtual bool interactive() { return false; }
	virtual void update(float time);
	virtual bool deleted();
protected:
	bool _deleted;
};

#endif