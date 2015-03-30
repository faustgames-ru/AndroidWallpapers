#include "Headers.h"

const unsigned long DEFAULT_BLENDING_TIME = 150;

BaseWarrior::BaseWarrior()
: BaseGameObject()
, Target()
, EnemyNexus()
, _initialized(false)
, _dead(false)
, _animation()
, _clipCurrent()
, _clipRun()
, _clipAttack()
, _clipDeath()
, _clipDead()
{}

BaseGameObject* BaseWarrior::constructor()
{
	return new BaseWarrior();
}

void BaseWarrior::interaction(BaseGameObject* object)
{
	if (Target != NULL)
	{
		if (Target->Health <= 0.0f)
			Target = NULL;
	}
	if ((Health > 0.0f) && (object->Health > 0.0f) && (position().distanceSquared(object->position()) < (GeometryRadius * GeometryRadius)))
	{
		Vector3 offset = (position() - object->position()) * 0.5f;
		_node->setTranslation(_node->getTranslation() + offset);
	}
	float distanceToTarget = Target != NULL ? Target->position().distanceSquared(position()) : FLT_MAX;
	if ((distanceToTarget > (ActionRadius * ActionRadius)) && (object->PlayerID != PlayerID)
		&& (object->Health > 0.0f))
	{
		Target = object;
	}
}

void BaseWarrior::update(float time)
{
	if (_dead)
	{
		if (!_clipDeath->isPlaying())
			switchToAnimation(_clipDead, AnimationClip::REPEAT_INDEFINITE, 0);
		return;
	}

	BaseGameObject::update(time);
	updateAnimationState();
	if (Health <= 0.0f)
	{	
		switchToAnimation(_clipDeath, 1.0f, DEFAULT_BLENDING_TIME);
		_dead = true;
	}
	else
	{
		if (Target == NULL)
			Target = EnemyNexus;

		if (Target != NULL)
		{
			Vector3 tPos = Target->position();
			_movementController._target = OpenSteer::Vec3(tPos.x, tPos.y, tPos.z);

			OpenSteer::Vec3 forward = _movementController.forward();
			Vector3 objectDir = _node->getForwardVectorWorld().normalize();
			Matrix rot;
			Matrix::createLookAt(0.0f, 0.0f, 0.0f, forward.x, forward.y, -forward.z, 0.0f, 1.0f, 0.0f, &rot);
			_node->setRotation(rot);
			objectDir = _node->getForwardVectorWorld().normalize();

			float radius = ((ActionRadius + Target->GeometryRadius) * (ActionRadius + Target->GeometryRadius));
			float distance = Target->position().distanceSquared(position());
			if (radius < distance)
			{
				switchToAnimation(_clipRun, AnimationClip::REPEAT_INDEFINITE, DEFAULT_BLENDING_TIME);
				_movementController.update(0.0f, time * 0.001f);
				OpenSteer::Vec3 pos = _movementController.position();
				_node->setTranslation(Vector3(pos.x, pos.y, pos.z));
			}
			else
			{
				switchToAnimation(_clipAttack, AnimationClip::REPEAT_INDEFINITE, DEFAULT_BLENDING_TIME);
				_damageTimer += time;
				if ((_damageTimer > DamageTime) && (Target != NULL))
				{
					Target->Health -= Damage;
					_damageTimer = 0.0f;
				}
			}
			
			
		}
	}
}

//const float ROTATION_FACTOR = 0.002f;
//const float MOVE_FACTOR = 0.0005f;

//Vector3 targetDir = Vector3(Target->position() - position()).normalize();
//Vector3 objectDir = _node->getForwardVectorWorld().normalize();
//float targetAngle = Vector3::angle(objectDir, targetDir);

//float rotationAngle = (targetAngle > (time * ROTATION_FACTOR)) ? time * ROTATION_FACTOR : targetAngle;
//Vector3 rightVector = _node->getRightVectorWorld().normalize();
//rotationAngle = copysignf(1.0f, Vector3::dot(rightVector, targetDir)) * rotationAngle;
//Vector3 vAxis;
//float currentAngle = _node->getRotation(&vAxis) + rotationAngle;
//while (currentAngle > 2.0f * MATH_PI) currentAngle -= 2.0f * MATH_PI;
//while (currentAngle < 0.0f) currentAngle += 2.0f * MATH_PI;
//_node->setRotation(Vector3::unitY(), currentAngle);
//_movementController.setForward(OpenSteer::Vec3(-objectDir.x, -objectDir.y, -objectDir.z));

//_node->setTranslation(_node->getTranslation() + targetDir * MOVE_FACTOR * time);
//_movementController.applySteeringForce(OpenSteer::Vec3(targetDir.x, targetDir.y, targetDir.z) * MOVE_FACTOR, time * 0.001f);

void BaseWarrior::updateAnimationState()
{
	//initialization
	if (!_initialized)
	{
		_animation = _node->getAnimation("animations"); GP_ASSERT(_animation);
		_clipRun = _animation->getClip("run"); GP_ASSERT(_clipRun);
		_clipAttack = _animation->getClip("attack"); GP_ASSERT(_clipAttack);
		_clipDeath = _animation->getClip("death"); GP_ASSERT(_clipDeath);
		_clipDead = _animation->getClip("dead"); GP_ASSERT(_clipDead);
		_initialized = true;
	}
}

void BaseWarrior::switchToAnimation(AnimationClip* clip, float repeatCount, unsigned long blendingTime)
{
	if ((_clipCurrent != clip) || (_clipCurrent == NULL) || (!_clipCurrent->isPlaying()))
	{
		clip->setSpeed(1.0f);
		clip->setRepeatCount(repeatCount);
		if (_clipCurrent && (blendingTime > 0))
			_clipCurrent->crossFade(clip, blendingTime);
		else
			clip->play();
		_clipCurrent = clip;
	}
}