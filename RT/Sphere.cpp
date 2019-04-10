#include "StdAfx.h"
#include "Sphere.h"
#include <glm\glm.hpp>


CSphere::CSphere(void)
{
}


CSphere::~CSphere(void)
{
}

float CSphere::intersect(CRay* ray)
{
	float t = -1;

	glm::vec3 v = ray->pos - center;
	float A = glm::dot(ray->dir, ray->dir);
	float B = 2.0 * glm::dot(v, ray->dir);
	float C = glm::dot(v, v) - radius * radius;

	float delta = B*B - 4*A*C;

	if (delta == 0)
		t = -B / (2 * A);
	else
		if (delta > 0)
		{
			float t1 = (-B - sqrt(delta)) / (2 * A);
			float t2 = (-B + sqrt(delta)) / (2 * A);
			
			if ((t1 < 0 && t2 < 0) || (t1 < 0 && t2 >= 0))
				t = t2;
			else
			if (t1 >= 0 && t2 >= 0)
				t = t1;
		}
		return t;
}

glm::vec3 CSphere::normalVector(glm::vec3 intersectionPoint)
{
	glm::vec3 n = glm::normalize(intersectionPoint - center);
	return n;
}
