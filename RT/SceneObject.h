#pragma once
#include <glm\glm.hpp>
class CSceneObject
{
public:
	CSceneObject(void);
	~CSceneObject(void);

	glm::vec3 specular;
	glm::vec3 diffuse;
	glm::vec3 ambient;

	float reflect;
	float shininess;

	virtual float intersect( CRay* ray ) = 0;
	virtual glm::vec3 normalVector(glm::vec3 intersectionPoint) = 0;
};

