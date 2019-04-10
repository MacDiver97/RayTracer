#pragma once
#include <glm\glm.hpp>
class CTriangle : public CSceneObject
{
public:
	CTriangle(void);
	~CTriangle(void);

	//Wierzcholek 1, 2, 3

	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;

	virtual float intersect(CRay* ray);
	virtual glm::vec3 normalVector(glm::vec3 intersectionPoint);
};

