#include "stdafx.h"
#include "BlobDetector.h"
#include <iostream>


BlobDetector::BlobDetector(void)
{
	storage = cvCreateMemStorage(0);
	defectStorage = cvCreateMemStorage(0);
}


BlobDetector::~BlobDetector(void)
{
	cvClearMemStorage(storage);
	cvReleaseMemStorage(&storage);
	cvClearMemStorage(defectStorage);
	cvReleaseMemStorage(&defectStorage);
	free(hull);
	free(points);
}

CvSeq * BlobDetector::findContours(IplImage * inImage, int mode)
{
	CvContour * contours;
	IplImage * imageTemp = cvCreateImage(cvSize(inImage->width, inImage->height), inImage->depth, inImage->nChannels);
	cvCopy(inImage, imageTemp);
	cvFindContours(imageTemp, storage, (CvSeq**)&contours, 88, mode);
	cvReleaseImage(&imageTemp);

	return (CvSeq*)contours;
}

CvSeq * BlobDetector::findBiggestContour(CvSeq * contours)
{
	CvContour * biggest = 0;

	CvRect max;
	max.width=0;
	max.height=0;



	std::cout << "Wtf?? " << std::endl;
	while (contours)
	{
		CvRect area = cvBoundingRect(contours, 1);
		if (area.height * area.width > max.height * max.width)
		{
			max = area;
			biggest = (CvContour *)contours;
		}
		contours = contours->h_next;
	}
	if (biggest)
	{
		biggest->h_next = 0;
		biggest->v_next = 0;
		biggest->h_prev = 0;
		biggest->v_prev = 0;
	}
	return (CvSeq *)biggest;
}

CvSeq * BlobDetector::findSmallestContour(CvSeq * contours)
{
	CvContour * smallest = 0;

	CvRect min;
	min.width=50000;
	min.height=50000;

	while (contours)
	{
		CvRect area = cvBoundingRect(contours, 1);
		if (area.height * area.width < min.height * min.width)
		{
			min = area;
			smallest = (CvContour *)contours;
		}
		contours = contours->h_next;
	}
	if (smallest)
	{
		smallest->h_next = 0;
		smallest->v_next = 0;
		smallest->h_prev = 0;
		smallest->v_prev = 0;
	}
	return (CvSeq *)smallest;
}

CvSeq * BlobDetector::approximateContour(CvSeq * contour, int percent, CvMemStorage *storage)
{
	if (contour != 0)
		return cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, percent);
	return 0;
}

CvPoint * BlobDetector::calculateConvexityPoints(CvSeq * contour)
{

	points = (CvPoint*)malloc(sizeof(CvPoint) * contour->total);
	cvCvtSeqToArray(contour, points, CV_WHOLE_SEQ);
	hull = (int *)malloc(sizeof(int)*contour->total);
	pointMat = cvMat( 1, contour->total, CV_32SC2, points);
    hullMatrix = cvMat( 1, contour->total, CV_32SC1, hull );
	cvConvexHull2( &pointMat, &hullMatrix, CV_CLOCKWISE, 0 );
	int hullsize = hullMatrix.cols;
	CvPoint * convexityPoints = (CvPoint *)malloc(sizeof(CvPoint)*contour->total);


	for (int i = 0; i < hullsize; ++i)
	{
		convexityPoints[i] = points[hull[i]];
	}
	//free(points);
	//free(hull);1
	return convexityPoints;
}

void BlobDetector::calculateConvexityDefect(CvSeq * contour)
{
	defects = cvConvexityDefects(contour, &hullMatrix, defectStorage);
	defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect) * defects->total);
	cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
}

CvMat * BlobDetector::getHullMatrix()
{
	return &hullMatrix;
}