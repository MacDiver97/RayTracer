#pragma once
#include <glm\glm.hpp>
class CScene
{
public:

	CCamera cam;

	std::vector<CSceneObject*> mObjects;

	std::vector<CLight*> mLights;
	CScene(void);
	~CScene(void);

	int importScene(std::string fname);
};

