#include "Headers.h"

BaseGameObject* AlbiriaWarrior::constructor()
{
	return new AlbiriaWarrior();
}

AlbiriaWarrior::AlbiriaWarrior()
: BaseWarrior()
, _altitude(0.0f)
, _fireListener(*this)
, _rechangeListener(*this)
{
}

void AlbiriaWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = ALBIRIA_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 1.5f;
	_altitude = 3.75f;

	if (_unitAnimation.size() > 0)
	{
		AnimationClip* clip = (*_unitAnimation[0]->_clips)[UnitAnimation::Attack];
		clip->addBeginListener(&_rechangeListener);
		clip->addListener(&_fireListener, (int)(0.25f * (float)clip->getDuration()));
	}
}

void AlbiriaWarrior::update(float time)
{
	BaseWarrior::update(time);
	if (_dead)
	{
		_altitude = (_altitude > 0.0f) ? _altitude - 0.002f * time : 0.0f;
	}
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y + _altitude, pos.z));
	/*Node* holder = _node->findNode("Bip001 Xtra02");
	Node* bullet = _node->findNode("bullet");
	Matrix mRes;
	Matrix mScale;
	Matrix::createScale(3, 3, 3, &mScale);
	Matrix::multiply(holder->getWorldMatrix(), mScale, &mRes);
	bullet->set(mRes);
	if (_dead)
	{
		_altitude = (_altitude > 0.0f) ? _altitude - 0.002f * time : 0.0f;
	}
	OpenSteer::Vec3 pos = _movementController.position(); 
	_node->setTranslation(Vector3(pos.x, pos.y + _altitude, pos.z));
	if (!_dead)
	{
		Quaternion rot;
		Quaternion::multiply(_node->getRotation(), Quaternion(Vector3(0.0f, 1.0f, 0.0f), MATH_PI), &rot);
		_node->setRotation(rot);
	}*/
}

void AlbiriaWarrior::fire()
{
	/*Node* bullet = _node->findNode("bullet");
	Node* holder = _node->findNode("Bip001 Xtra02");

	Matrix mRes;
	Matrix mScale;
	Matrix mHolder;
	Matrix mCharacterRoot = _node->getWorldMatrix();
	Matrix::createScale(3, 3, 3, &mScale);
	Matrix::multiply(holder->getWorldMatrix(), mScale, &mHolder);
	Matrix::multiply(mCharacterRoot, mHolder, &mRes);
	
	BaseBullet* object = new BaseBullet();
	object->Target = Target;
	object->init(*_manager, bullet, PlayerID, mRes);

	bullet->setEnabled(false);*/
}

void AlbiriaWarrior::recharge()
{
	/*Node* bullet = _node->findNode("bullet");
	bullet->setEnabled(true);*/
}