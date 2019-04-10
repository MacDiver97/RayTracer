#pragma once
#include <glm\glm.hpp>
#include "FreeImage.h"
#include <opencv2/opencv.hpp>

class CBitmap
{
public:

	int mWidth;
	int mHeight;
	FIBITMAP * mBitmap;
	cv::Mat mat;
	//data

	CBitmap(void);
	CBitmap(int x, int y);
	~CBitmap(void);

	bool init(int width, int height);
	bool setPixel(int x, int y, glm::vec3 color);
	bool setPixelMat(int x, int y, glm::vec3 color);
	void getPixel(int x, int y, glm::vec3 & color);
	void getPixelMat(int x, int y, glm::vec3 & color);
	bool save(std::string fname);
	bool saveMat(std::string fname);
	
};

