#ifndef UTILS__H_
#define UTILS__H_

void createCharacterRotationMatrix(Vector3 forward, Matrix* rotation);

const Vector3 ProjectToPlane(Camera* camera, int x, int y, Vector3 normal, Vector3 pointOnPlane);
const Vector3 ProjectToZeroPlane(Camera* camera, int x, int y);
bool ProjectRayIntersectSphere(Camera* camera, int x, int y, Vector3 point, float radiusSquared);


#endif