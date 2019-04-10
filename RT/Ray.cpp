#include "StdAfx.h"
#include "Ray.h"


CRay::CRay(void)
{
}

void CRay::setRay(float i, float j, const CCamera & cam)
{
	pos = cam.pos;   // pozycja promienia
	glm::vec3 look = glm::normalize(cam.target - cam.pos);

	glm::vec3 u = glm::normalize( glm::cross(cam.up, look) );
	glm::vec3 v = glm::normalize(glm::cross(u, look));
	glm::vec3 o = look * float((cam.mWidth / (2.0f*tanf(cam.FOV / 2.0f))))
		- float((cam.mWidth / 2.0f)) * u - float((cam.mHeight/2.0f))*v;
	
	dir = glm::normalize(glm::mat3(u, v, o) * glm::vec3(i, j, 1.0f));   //kierunek promienia
}

CRay::~CRay(void)
{
}


