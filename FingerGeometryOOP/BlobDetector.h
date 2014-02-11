#pragma once
#include <cv.h>

class FingerDetector;

class BlobDetector
{
	friend class FingerDetector;
	CvMemStorage * storage;
	CvMemStorage * defectStorage;
	CvSeq * defects;
	CvConvexityDefect * defectArray;
	CvMat hullMatrix;
	CvMat pointMat;
	CvPoint * points;
	int * hull;
public:
	BlobDetector(void);
	~BlobDetector(void);
	/***
	* CvSeq * findContours(IplImage *);
	* param 1 : IplImage * -> detect contours from this image
	* param 2 : int -> contour list return type
	* Description : finds the contours in an image and returns these contours under a CvSeq*
	***/
	CvSeq * findContours(IplImage *, int = 1);
	/***
	* CvSeq * findBiggestContour(CvSeq *);
	* param 1 : The cvSeq * that was returned by the previous function
	* Description : finds the biggest contour in a given CvSeq *
	***/
	CvSeq * findBiggestContour(CvSeq *);
	/***
	* CvSeq * findSmallestContour(CvSeq *);
	* param 1 : The cvSeq * that was returned by the previous function
	* Description : finds the smallest contour in a given CvSeq *
	***/
	CvSeq * findSmallestContour(CvSeq *);
	
	/***
	* CvSeq * approximateContour(CvSeq*, int);
	* param1 : CvSeq * -> contour to be approximated
	* int -> percentage of approximation
	* CvMemStorage *-> storage section of the approximated contour
	* Description : approximates the contour using Douglas-Peucker algorithm
	***/
	CvSeq * approximateContour(CvSeq *, int, CvMemStorage *);

	CvPoint * calculateConvexityPoints(CvSeq *);

	void calculateConvexityDefect(CvSeq *);

	CvMat * getHullMatrix();
};

