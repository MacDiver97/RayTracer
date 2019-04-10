#include "StdAfx.h"
#include "Scene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define PI 3.14159265358979323846


CScene::CScene(void)
{
}


CScene::~CScene(void)
{
}

int CScene::importScene(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);

	if (file.fail())
		return 1;

	std::string line;
	std::string type;

	while (getline(file, line))
	{
		std::istringstream iss(line);

		type = "";
		iss >> type;

		if (type.compare("cam_width") == 0) {
			iss >> cam.mWidth;
		}
		else
			if (type.compare("cam_height") == 0) {
				iss >> cam.mHeight;
			}
			else
				if (type.compare("cam_FOV") == 0) {
					iss >> cam.FOV;
					float result = PI / 180.0f;
					cam.FOV = result * cam.FOV;
				}
				else
					if (type.compare("cam_pos") == 0) {
						iss >> cam.pos.x;
						iss >> cam.pos.y;
						cam.pos.y *= 1; //-1
						iss >> cam.pos.z;
					}
					else
						if (type.compare("cam_target") == 0) {
							iss >> cam.target.x;
							iss >> cam.target.y;
							iss >> cam.target.z;
						}
						else
							if (type.compare("cam_up") == 0) {
								iss >> cam.up.x;
								iss >> cam.up.y;
								iss >> cam.up.z;
							}
							else
								if (type.compare("sphere") == 0) {
									CSphere * sphere = new CSphere();
									iss >> sphere->radius;

									iss >> sphere->center.x;
									iss >> sphere->center.y;
									sphere->center.y *= 1; //-1
									iss >> sphere->center.z;

									iss >> sphere->reflect;

									iss >> sphere->ambient.x;
									iss >> sphere->ambient.y;
									iss >> sphere->ambient.z;

									iss >> sphere->diffuse.x;
									iss >> sphere->diffuse.y;
									iss >> sphere->diffuse.z;

									iss >> sphere->specular.x;
									iss >> sphere->specular.y;
									iss >> sphere->specular.z;

									iss >> sphere->shininess;

									mObjects.push_back(sphere);  //dodanie obiektu do sceny
								}
								else
									if (type.compare("triangle") == 0) {
										CTriangle *	triangle = new CTriangle();

										iss >> triangle->p1.x;
										iss >> triangle->p1.y;
										triangle->p1.y *= 1; //-1
										iss >> triangle->p1.z;

										iss >> triangle->p2.x;
										iss >> triangle->p2.y;
										triangle->p2.y *= 1; //-1
										iss >> triangle->p2.z;

										iss >> triangle->p3.x;
										iss >> triangle->p3.y;
										triangle->p3.y *= 1; //-1
										iss >> triangle->p3.z;

										iss >> triangle->reflect;

										iss >> triangle->ambient.x;
										iss >> triangle->ambient.y;
										iss >> triangle->ambient.z;

										iss >> triangle->diffuse.x;
										iss >> triangle->diffuse.y;
										iss >> triangle->diffuse.z;

										iss >> triangle->specular.x;
										iss >> triangle->specular.y;
										iss >> triangle->specular.z;

										iss >> triangle->shininess;

										mObjects.push_back(triangle);  //dodanie obiektu do sceny
									}
									else
										if (type.compare("light") == 0) {
											CLight * light = new CLight();

											iss >> light->pos.x;
											iss >> light->pos.y;
											light->pos.y *= 1; //-1
											iss >> light->pos.z;

											iss >> light->ambient.x;
											iss >> light->ambient.y;
											iss >> light->ambient.z;

											iss >> light->diffuse.x;
											iss >> light->diffuse.y;
											iss >> light->diffuse.z;

											iss >> light->specular.x;
											iss >> light->specular.y;
											iss >> light->specular.z;

											mLights.push_back(light); //dodanie œwiat³a do sceny
										}
	}
	file.close();

	return 0;
}

