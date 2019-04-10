#pragma once
#include <glm\glm.hpp>

class CSphere : public CSceneObject
{
public:
	CSphere(void);
	~CSphere(void);

	
	glm::vec3 center;
	float radius;

	virtual float intersect(CRay* ray);
	virtual glm::vec3 normalVector(glm::vec3 intersectionPoint);
};

