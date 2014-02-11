#include "stdafx.h"
#include <iostream>
#include "FilterManager.h"


FilterManager::FilterManager(void)
{
}


FilterManager::~FilterManager(void)
{
}

IplImage * FilterManager::resize(IplImage * inImage, CvSize newSize)
{
	if (inImage)
	{
		IplImage * outImage = cvCreateImage(newSize, inImage->depth, inImage->nChannels);

		cvResize(inImage, outImage);
		return outImage;
	}
	return 0;
}

IplImage * FilterManager::threshold(IplImage * inImage, int min, int max, int type)
{
	if (inImage)
	{
		IplImage * outImage = cvCreateImage(cvSize(inImage->width, inImage->height), inImage->depth, inImage->nChannels);
		cvThreshold(inImage, outImage, min, max, type);
		return outImage;
	}
	return 0;
}

IplImage * FilterManager::smooth(IplImage * inImage, int type, int kernelX, int kernelY, float sigmaX, float sigmaY)
{
	if (inImage)
	{
		IplImage * outImage = cvCreateImage(cvSize(inImage->width, inImage->height), inImage->depth, inImage->nChannels);
		cvSmooth(inImage, outImage, type, kernelX, kernelY, sigmaX, sigmaY);
		return outImage;
	}
	return 0;	
}

IplImage * FilterManager::combineImages(IplImage * threshed, IplImage * grayscale)
{
	if (threshed && grayscale
		&& threshed->nChannels == grayscale->nChannels
		&& threshed->width == grayscale->width
		&& threshed->height == grayscale->height)
	{
		IplImage * combined = cvCreateImage(cvSize(threshed->width, threshed->height), threshed->depth, threshed->nChannels);
		for (int i = 0 ; i < threshed->width; ++i)
		{
			for (int j = 0 ; j < threshed->height ; ++j)
			{
				int index = j * threshed->width + i;
				if (threshed->imageData[index] != 0)
				{
					combined->imageData[index] = grayscale->imageData[index];
				}
			}
		}
		return combined;
	}
	return 0;
}

cv::Mat FilterManager::createGaborKernel(int kernelSize, double sgm, double tht, double lambda, double phaseOffset)
{
	int hks = (kernelSize-1)/2;
	double theta = tht*CV_PI/180;
	double psi = phaseOffset*CV_PI/180;
	double del = 2.0/(kernelSize-1);
	lambda = 0.5 + lambda / 100.0;
	double lmbd = lambda;
	double sigma = sgm / kernelSize;
	double x_theta;
	double y_theta;
	cv::Mat kernel(kernelSize, kernelSize, CV_32F);
	for (int y=-hks; y<=hks; y++)
	{
		for (int x=-hks; x<=hks; x++)
		{
			x_theta = x*del*cos(theta)+y*del*sin(theta);
			y_theta = -x*del*sin(theta)+y*del*cos(theta);
			kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
		}
	}
	return kernel;
}