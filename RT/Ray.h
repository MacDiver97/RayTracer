#pragma once
#include <glm\glm.hpp>
#include "Camera.h"

class CRay
{
public:
	glm::vec3 pos;
	glm::vec3 dir;

	void setRay(float i, float j, const CCamera & cam);
	CRay(void);
	~CRay(void);

};

