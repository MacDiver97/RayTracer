#include "StdAfx.h"
#include "Bitmap.h"
#include <iostream>
CBitmap::CBitmap(void) :
	mWidth(1000),
	mHeight(800),
	mat(mHeight, mWidth, CV_8UC3, cv::Scalar(0, 0, 0))
{
}

CBitmap::CBitmap(int x, int y) :
	mWidth(x),
	mHeight(y),
	mat(mHeight, mWidth, CV_8UC3, cv::Scalar(0, 0, 0))
{
}

CBitmap::~CBitmap(void)
{
}

bool CBitmap::init(int width, int height)
{
	mBitmap = FreeImage_AllocateT(FIT_BITMAP, width, height, 32);
	return true;
}

bool CBitmap::setPixel(int x, int y, glm::vec3 color)
{
	RGBQUAD quad;

	color.x = color.x*255.0f;
	color.y = color.y*255.0f;
	color.z = color.z*255.0f;

	quad.rgbRed = color.x;
	quad.rgbGreen = color.y;
	quad.rgbBlue = color.z;

	//if (y > 500 && y < 508)
		//std::cout << "multiplied " << x << "\t" << y << "\t" << color[0] << "\t" << color[1] << "\t" << color[2] << std::endl;

	FreeImage_SetPixelColor(mBitmap, x, y, &quad);
	return false;
}

bool CBitmap::setPixelMat(int x, int y, glm::vec3 color)
{
	color.x = color.x*255.0f;
	color.y = color.y*255.0f;
	color.z = color.z*255.0f;

	mat.at<cv::Vec3b>(y, x)[2] = color.x;
	mat.at<cv::Vec3b>(y, x)[1] = color.y;
	mat.at<cv::Vec3b>(y, x)[0] = color.z;
	return false;
}

void CBitmap::getPixel(int x, int y, glm::vec3 & color)
{
	RGBQUAD quad;
	FreeImage_GetPixelColor(mBitmap, x, y, &quad);
	color.x = quad.rgbRed;
	color.y = quad.rgbGreen;
	color.z = quad.rgbBlue;
}

void CBitmap::getPixelMat(int x, int y, glm::vec3 & color)
{	
	color.x = mat.at<cv::Vec3b>(y, x)[2];
	color.y = mat.at<cv::Vec3b>(y, x)[1];
	color.z = mat.at<cv::Vec3b>(y, x)[0];
}

bool CBitmap::save(std::string fname)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType( fname.c_str() );

	FreeImage_Save( FIF_BMP, mBitmap, fname.c_str() );

	return false;
}

bool CBitmap::saveMat(std::string fname)
{
	cv::imwrite(fname, mat);
	return false;
}
