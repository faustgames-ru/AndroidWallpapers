#include "BaseWarrior.h"

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
			const float ROTATION_FACTOR = 0.002f;
			const float MOVE_FACTOR = 0.002f;

			Vector3 targetDir = Vector3(Target->position() - position()).normalize();
			Vector3 objectDir = _node->getForwardVectorWorld().normalize();
			float targetAngle = Vector3::angle(objectDir, targetDir);

			//if (targetAngle > 0.05f)
			{
				float rotationAngle = (targetAngle > (time * ROTATION_FACTOR)) ? time * ROTATION_FACTOR : targetAngle;
				Vector3 rightVector = _node->getRightVectorWorld().normalize();
				rotationAngle = copysignf(1.0f, Vector3::dot(rightVector, targetDir)) * rotationAngle;
				Vector3 vAxis;
				float currentAngle = _node->getRotation(&vAxis) + rotationAngle;
				while (currentAngle > 2.0f * MATH_PI) currentAngle -= 2.0f * MATH_PI;
				while (currentAngle < 0.0f) currentAngle += 2.0f * MATH_PI;
				_node->setRotation(Vector3::unitY(), currentAngle);
			}

			float radius = ((ActionRadius + Target->GeometryRadius) * (ActionRadius + Target->GeometryRadius));
			float distance = Target->position().distanceSquared(position());
			if (radius < distance)
			{
				//if (targetAngle < 0.05f)
				{
					switchToAnimation(_clipRun, AnimationClip::REPEAT_INDEFINITE, DEFAULT_BLENDING_TIME);
					_node->setTranslation(_node->getTranslation() + targetDir * MOVE_FACTOR * time);
				}
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