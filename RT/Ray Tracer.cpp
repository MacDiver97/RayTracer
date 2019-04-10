// Ray Tracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glm\glm.hpp>
#include "FreeImage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include <random>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#define PI 3.14159265358979323846
#define AAx8 1
#define AAx16 2

using namespace glm;
using namespace std;
using namespace cv;

struct Output {
	float energy;
	int tree;
	float color[3];
};

struct Memory{
	CSceneObject * obj;
	float t;
};

int run(CScene* scene, CBitmap& img, int aa = 0);
int rayTrace(CRay &ray, CScene* scene, Output* res);

void gamma(float * color)
{
	for (int i = 0; i < 3; ++i)
	{
		if (color[i] < 0.00304)
			color[i] *= 12.92;
		else
			color[i] = 1.055 * pow(color[i], 1.0f / 2.4f) - 0.055;
	}
}

Memory findIntersection(CRay & ray, CScene * scene) {

	float pom = 0;
	Memory memObject; 
		memObject.t = 0;

	for (int i = 0; i < scene->mObjects.size(); ++i) {    // dla ka¿dego obiektu

		memObject.t = scene->mObjects[i]->intersect(&ray);    //wykrywanie przeciec promieni z obiektami

		if (memObject.t > 0.00001)    //jeœli znaleziono przeciecie
		{
			if (memObject.t < pom || pom == 0)
			{
				pom = memObject.t;
				memObject.obj = scene->mObjects[i];

				continue;
			}
		}
		else
			if (memObject.t < 0 && pom == 0)   //jeœli nie znaleziono przeciecia
			{
				memObject.obj = NULL;
			}
	}

	if (pom == 0)
		memObject.t = -1.0f;
	else
		memObject.t = pom;

	return memObject;
}

vec3 lightEquation(vec3 intersectionPoint, Memory& mem, CScene * scene, int lightc) {

	vec3 newcolor(0.0f, 0.0f, 0.0f);

	//float iamb = mem.obj->ambient.x * scene->mLights[lightc]->ambient.x;

	vec3 l = normalize(scene->mLights[lightc]->pos - intersectionPoint);
	vec3 n = mem.obj->normalVector(intersectionPoint);
	vec3 v = normalize(scene->cam.pos - intersectionPoint);
	vec3 h = normalize(l + v);

	float iamb = mem.obj->ambient.x * scene->mLights[lightc]->ambient.x;
	float idif = mem.obj->diffuse.x * scene->mLights[lightc]->diffuse.x * clamp(dot(l, n),0.0f, 1.0f);
	float ispe = mem.obj->specular.x * scene->mLights[lightc]->specular.x * pow(clamp(dot(n, h), 0.0f, 1.0f), mem.obj->shininess);

	newcolor.x = iamb + idif + ispe;

	iamb = mem.obj->ambient.y * scene->mLights[lightc]->ambient.y;
	idif = mem.obj->diffuse.y * scene->mLights[lightc]->diffuse.y * clamp(dot(l, n), 0.0f, 1.0f);
	ispe = mem.obj->specular.y * scene->mLights[lightc]->specular.y * pow(clamp(dot(n, h), 0.0f, 1.0f), mem.obj->shininess);

	newcolor.y = iamb + idif + ispe;

	iamb = mem.obj->ambient.z * scene->mLights[lightc]->ambient.z;
	idif = mem.obj->diffuse.z * scene->mLights[lightc]->diffuse.z * clamp(dot(l, n), 0.0f, 1.0f);
	ispe = mem.obj->specular.z * scene->mLights[lightc]->specular.z * pow(clamp(dot(n, h), 0.0f, 1.0f), mem.obj->shininess);

	newcolor.z = iamb + idif + ispe;

	return newcolor;
}

ostream& operator<< (ostream& os, const vec3& obj) {
	os << obj.x << "\t" << obj.y << "\t" << obj.z << '\n';
	return os;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	CScene scene;

	//import sceny z pliku 
	if (scene.importScene("scena.txt"))
	{
		cout << "Import scene from txt failed!!!" << endl;
		return -1;
	}	

	CBitmap img(scene.cam.mWidth,scene.cam.mHeight);

	run(&scene, img, AAx16);  //petla ray tracer'a 0 - no AA, AAx8, AAx16	

	img.saveMat("outmap.bmp");

	return 0;
}

// G³ówna pêtla ray tracer'a
int run(CScene* scene, CBitmap& img, int aa) {

	Output * out = new Output();
	CRay ray;

	for (int y = 0; y < scene->cam.mHeight; y++) {
		for (int x = 0; x < scene->cam.mWidth; x++) {

			ray.setRay(x+0.5, y+0.5, scene->cam);   // wyznaczenie parametrów promienia

			out->energy = 1.0f;
			out->color[0] = 0.0f;
			out->color[1] = 0.0f;
			out->color[2] = 0.0f;
			rayTrace(ray, scene, out);		//œledzenie pojedynczego promienia

			out->color[0] = clamp(out->color[0], 0.0f, 1.0f);
			out->color[1] = clamp(out->color[1], 0.0f, 1.0f);
			out->color[2] = clamp(out->color[2], 0.0f, 1.0f);

			gamma(out->color);

			//img.setPixel(x, y, glm::vec3(out->color[0], out->color[1], out->color[2]));    //ustawianie wartoœci piksela
			img.setPixelMat(x, y, glm::vec3(out->color[0], out->color[1], out->color[2]));
		}
	}
	
	//anti-aliasing
	if (aa > 0)
	{
		//Canny edge detector
		Mat src = img.mat;
		Mat src_gray;
		Mat dst, detected_edges;
		int ratio = 3;
		int kernel_size = 3;
		int lowThreshold = 10;

		dst.create(src.size(), src.type());
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		blur(src_gray, detected_edges, Size(3, 3));
		Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);;
		dst = Scalar::all(0);
		src.copyTo(dst, detected_edges);
		int xx, yy;
		//adaptive anti-aliasing x8
		for (int y = 0; y < scene->cam.mHeight; y++) {
			for (int x = 0; x < scene->cam.mWidth; x++) {
				if ((dst.at<cv::Vec3b>(y, x)[2] > 0 || dst.at<cv::Vec3b>(y, x)[1] > 0 || dst.at<cv::Vec3b>(y, x)[0] > 0)
					&& x < scene->cam.mWidth - 1
					&& y < scene->cam.mHeight - 1
					&& x > 1
					&& y > 1)
				{
					for (int m = 0; m < 9; m++)
					{
						switch (m) {
						case 0:
							xx = x - 1;
							yy = y - 1;
							break;
						case 1:
							xx = x;
							yy = y - 1;
							break;
						case 2:
							xx = x + 1;
							yy = y - 1;
							break;
						case 3:
							xx = x - 1;
							yy = y;
							break;
						case 4:
							xx = x;
							yy = y;
							break;
						case 5:
							xx = x + 1;
							yy = y;
							break;
						case 6:
							xx = x - 1;
							yy = y + 1;
							break;
						case 7:
							xx = x;
							yy = y + 1;
							break;
						case 8:
							xx = x + 1;
							yy = y + 1;
							break;

						}
						vec3 meanColor(0.0f, 0.0f, 0.0f);
						for (int k = 0; k < 8; k++)
						{
							// wyznaczenie parametrów promienia
							switch (k) {
							case 0:
								ray.setRay(xx, yy, scene->cam);
								break;
							case 1:
								ray.setRay(xx + 0.5f, yy, scene->cam);
								break;
							case 2:
								ray.setRay(xx + 1.0f, yy, scene->cam);
								break;
							case 3:
								ray.setRay(xx, yy + 0.5f, scene->cam);
								break;
							case 4:
								ray.setRay(xx, yy + 1.0f, scene->cam);
								break;
							case 5:
								ray.setRay(xx + 1.0f, yy + 0.5f, scene->cam);
								break;
							case 6:
								ray.setRay(xx + 0.5f, yy + 1.0f, scene->cam);
								break;
							case 7:
								ray.setRay(xx + 1.0f, yy + 1.0f, scene->cam);
								break;
							}
							out->energy = 1.0f;
							out->color[0] = 0.0f;
							out->color[1] = 0.0f;
							out->color[2] = 0.0f;
							//œledzenie pojedynczego promienia
							rayTrace(ray, scene, out);
							meanColor.x += clamp(out->color[0], 0.0f, 1.0f);
							meanColor.y += clamp(out->color[1], 0.0f, 1.0f);
							meanColor.z += clamp(out->color[2], 0.0f, 1.0f);
						}
						meanColor.x /= 8.0f;
						meanColor.y /= 8.0f;
						meanColor.z /= 8.0f;
						out->color[0] = meanColor.x;
						out->color[1] = meanColor.y;
						out->color[2] = meanColor.z;

						gamma(out->color);

						//ustawianie wartoœci piksela
						img.setPixelMat(xx, yy, glm::vec3(out->color[0], out->color[1], out->color[2]));

					}
				}
			}
		}

		//x16
		if (aa > 1)
		{
			src = img.mat;

			ratio = 3;
			kernel_size = 3;
			lowThreshold = 10;

			dst.create(src.size(), src.type());
			cvtColor(src, src_gray, COLOR_BGR2GRAY);
			blur(src_gray, detected_edges, Size(3, 3));
			Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);;
			dst = Scalar::all(0);
			src.copyTo(dst, detected_edges);

			//adaptive anti-aliasing x16
			for (int y = 0; y < scene->cam.mHeight; y++) {
				for (int x = 0; x < scene->cam.mWidth; x++) {
					if ((dst.at<cv::Vec3b>(y, x)[2] > 0 || dst.at<cv::Vec3b>(y, x)[1] > 0 || dst.at<cv::Vec3b>(y, x)[0] > 0)
						&& x < scene->cam.mWidth - 1
						&& y < scene->cam.mHeight - 1 
						&& x > 1
						&& y > 1)
					{
						for (int m = 0; m < 9; m++)
						{
							switch (m) {
							case 0:
								xx = x - 1;
								yy = y - 1;
								break;
							case 1:
								xx = x;
								yy = y - 1;
								break;
							case 2:
								xx = x + 1;
								yy = y - 1;
								break;
							case 3:
								xx = x - 1;
								yy = y;
								break;
							case 4:
								xx = x;
								yy = y;
								break;
							case 5:
								xx = x + 1;
								yy = y;
								break;
							case 6:
								xx = x - 1;
								yy = y + 1;
								break;
							case 7:
								xx = x;
								yy = y + 1;
								break;
							case 8:
								xx = x + 1;
								yy = y + 1;
								break;

							}

							vec3 meanColor(0.0f, 0.0f, 0.0f);
							for (int k = 0; k < 16; k++)
							{
								// wyznaczenie parametrów promienia
								switch (k) {
								case 0:
									ray.setRay(xx + 0.25f, yy, scene->cam);
									break;
								case 1:
									ray.setRay(xx + 0.75f, yy, scene->cam);
									break;
								case 2:
									ray.setRay(xx, yy + 0.25f, scene->cam);
									break;
								case 3:
									ray.setRay(xx + 0.25f, yy + 0.25f, scene->cam);
									break;
								case 4:
									ray.setRay(xx + 0.5f, yy + 0.25f, scene->cam);
									break;
								case 5:
									ray.setRay(xx + 0.75f, yy + 0.25f, scene->cam);
									break;
								case 6:
									ray.setRay(xx + 1.0f, yy + 0.25f, scene->cam);
									break;
								case 7:
									ray.setRay(xx + 0.25f, yy + 0.5f, scene->cam);
									break;
								case 8:
									ray.setRay(xx + 0.75f, yy + 0.5f, scene->cam);
									break;
								case 9:
									ray.setRay(xx, yy + 0.75f, scene->cam);
									break;
								case 10:
									ray.setRay(xx + 0.25f, yy + 0.75f, scene->cam);
									break;
								case 11:
									ray.setRay(xx + 0.5f, yy + 0.75f, scene->cam);
									break;
								case 12:
									ray.setRay(xx + 0.75f, yy + 0.75f, scene->cam);
									break;
								case 13:
									ray.setRay(xx + 1.0f, yy + 0.75f, scene->cam);
									break;
								case 14:
									ray.setRay(xx + 0.25f, yy + 1.0f, scene->cam);
									break;
								case 15:
									ray.setRay(xx + 0.75f, yy + 1.0f, scene->cam);
									break;
								}
								out->energy = 1.0f;
								out->color[0] = 0.0f;
								out->color[1] = 0.0f;
								out->color[2] = 0.0f;
								//œledzenie pojedynczego promienia
								rayTrace(ray, scene, out);
								meanColor.x += clamp(out->color[0], 0.0f, 1.0f);
								meanColor.y += clamp(out->color[1], 0.0f, 1.0f);
								meanColor.z += clamp(out->color[2], 0.0f, 1.0f);
							}
							meanColor.x /= 16.0f;
							meanColor.y /= 16.0f;
							meanColor.z /= 16.0f;
							out->color[0] = meanColor.x;
							out->color[1] = meanColor.y;
							out->color[2] = meanColor.z;

							gamma(out->color);

							//ustawianie wartoœci piksela
							img.setPixelMat(xx, yy, glm::vec3(out->color[0], out->color[1], out->color[2]));
						}
					}
				}
			}
		}
	}

	delete out;
	return 0;
}

// Sledzenie pojedynczego promienia
int rayTrace(CRay &ray, CScene* scene, Output* res)
{
	Memory intersection;
	intersection = findIntersection(ray, scene);
	float v1, v2, v3;

	if (intersection.obj == NULL)
	{
		return 0;
	}
	else {
		for (int i = 0; i < scene->mLights.size(); ++i)
		{

			glm::vec3 intersectionPoint = ray.pos + intersection.t * ray.dir;
			int lights = 0;
			CRay shadowRay;
			Memory lightIntersection;
			shadowRay.pos = intersectionPoint;

				shadowRay.dir = glm::normalize(scene->mLights[i]->pos - intersectionPoint);
				shadowRay.pos = shadowRay.pos + 0.01f * shadowRay.dir;  //prevent self-shading
				lightIntersection = findIntersection(shadowRay, scene);
			
				if (lightIntersection.obj == NULL)
			//if (lights == shadowSmooth)
				{
				vec3 newcolor = lightEquation(intersectionPoint, intersection, scene, i);
				res->color[0] = res->color[0] + res->energy * newcolor.x;
				res->color[1] = res->color[1] + res->energy * newcolor.y;
				res->color[2] = res->color[2] + res->energy * newcolor.z;
				}
			else
			{
					res->color[0] = res->color[0] + res->energy * intersection.obj->ambient.x * scene->mLights[i]->ambient.x;
					res->color[1] = res->color[1] + res->energy * intersection.obj->ambient.y * scene->mLights[i]->ambient.y;
					res->color[2] = res->color[2] + res->energy * intersection.obj->ambient.z * scene->mLights[i]->ambient.z;
			}
		}
	}

	res->energy = res->energy - 0.5f;

	if (res->energy < 0.1f)
		return 0;

	// secondary ray
	if (intersection.obj->reflect == 1)
	{
		vec3 intersectionPoint = ray.pos + intersection.t * ray.dir;
		CRay secondaryRay;
		secondaryRay.pos = intersectionPoint;
		secondaryRay.dir = normalize(reflect(ray.dir, intersection.obj->normalVector(intersectionPoint)));
		secondaryRay.pos = secondaryRay.pos + 0.01f * secondaryRay.dir;

		rayTrace(secondaryRay, scene, res);
	}

	return 0;
}