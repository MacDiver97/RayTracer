#pragma once
#include <glm\glm.hpp>

class CCamera
{
public:

	int mWidth;
	int mHeight;

	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	float FOV;

	CCamera(void);
	~CCamera(void);
};

