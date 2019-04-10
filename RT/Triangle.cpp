#include "StdAfx.h"
#include "Triangle.h"


CTriangle::CTriangle(void)
{
}


CTriangle::~CTriangle(void)
{
}

float CTriangle::intersect(CRay* ray)
{
	float t = -1;

	glm::vec3 u = glm::normalize(p3 - p1);
	glm::vec3 v = glm::normalize(p2 - p1);
	glm::vec3 w = glm::normalize(glm::cross(u, v));

	float D = -(w.x * p1.x + w.y * p1.y + w.z * p1.z);

	t = -(w.x * ray->pos.x + w.y * ray->pos.y + w.z * ray->pos.z + D)
		/ (w.x * ray->dir.x + w.y * ray->dir.y + w.z * ray->dir.z);

	glm::vec3 P = ray->pos + t * ray->dir;

	glm::vec3 v0 = p3 - p1;
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = P - p1;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float m = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float n = (dot00 * dot12 - dot01 * dot02) * invDenom;

	if ( (m >= 0) && (n >= 0) && ((m + n) < 1) )
		return t;
	else
	{
		t = -1;
		return t;
	}
}

glm::vec3 CTriangle::normalVector(glm::vec3 intersectionPoint)
{
	glm::vec3 n = glm::normalize(glm::cross((p2 - p1),(p3 - p1))); //-1
	return n;
}
