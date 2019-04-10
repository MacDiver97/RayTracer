#pragma once
#include <glm\glm.hpp>
class CLight
{
public:
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 specular;
	glm::vec3 diffuse;

	CLight(void);
	~CLight(void);
};

