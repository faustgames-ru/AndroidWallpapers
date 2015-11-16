#include "Headers.h"

void createCharacterRotationMatrix(Vector3 forward, Matrix* rotation)
{
	Vector3 xaxis = forward;
	Vector3 yaxis = Vector3::unitY();
	Vector3 zaxis;
	Vector3::cross(yaxis, xaxis, &zaxis);
	zaxis.normalize();
	rotation->m[0] = xaxis.x;
	rotation->m[1] = yaxis.x;
	rotation->m[2] = zaxis.x;
	rotation->m[3] = 0.0f;

	rotation->m[4] = xaxis.y;
	rotation->m[5] = yaxis.y;
	rotation->m[6] = zaxis.y;
	rotation->m[7] = 0.0f;

	rotation->m[8] = xaxis.z;
	rotation->m[9] = yaxis.z;
	rotation->m[10] = zaxis.z;
	rotation->m[11] = 0.0f;

	rotation->m[12] = 0.0f;
	rotation->m[13] = 0.0f;
	rotation->m[14] = 0.0f;
	rotation->m[15] = 1.0f;
}

const Vector3 ProjectToPlane(Camera* camera, int x, int y, Vector3 normal, Vector3 pointOnPlane)
{
	Ray ray;
	camera->pickRay(Game::getInstance()->getViewport(), x, y, &ray);
	const float distance = Vector3::dot(pointOnPlane, normal);
	Plane plane(normal, -distance);

	Vector3 point;
	float collisionDistance = ray.intersects(plane);
	if (collisionDistance != Ray::INTERSECTS_NONE)
	{
		point.set(ray.getOrigin() + collisionDistance*ray.getDirection());
		return point;
	}
	else
		return Vector3::zero();
}

const Vector3 ProjectToZeroPlane(Camera* camera, int x, int y)
{
	return ProjectToPlane(camera, x, y, Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
}

bool ProjectRayIntersectSphere(Camera* camera, int x, int y, Vector3 point, float radiusSquared)
{
	const Vector3 p = ProjectToPlane(camera, x, y, Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	return p.distanceSquared(point) <= radiusSquared;
}