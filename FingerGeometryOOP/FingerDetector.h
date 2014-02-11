#pragma once
#include <vector>
#include <cv.h>
#include "Finger.h"
#include "BlobDetector.h"

class FingerDetector
{
private :
	std::vector<Finger*> fingers;
	CvMemStorage * storage;
	CvPoint getRealFingerTip(IplImage*);

	/***
	* getMinIndex : get the finger the closest in X to 0
	***/
	int getMinIndex();
	/***
	* getMaxIndex : get the finger the farthest in X from 0
	***/
	int getMaxIndex();

	/***
	* removeAtIndex : remove the finger in the fingers attribute at the specified index
	***/
	void removeAtIndex(int);
public :
	FingerDetector(void);
	~FingerDetector(void);
	/***
	* detectFingers : it will detect all the finger tips, the defect points so in between the fingers and estimate the trajectory of each finger
	* parameteres : CvSeq * -> Biggest or smallest contour found in the image
	*			  : CvSize -> get size of the image for better processing
	***/
	void detectFingers(BlobDetector *, CvSize, IplImage*);

	/***
	* differentiateDefectPoints : it differentiates the defect points whether it is the one before or after (on the X axis) the convexe point which represents the finger tip
	* parameters : CvPoint & -> reference to before point
	*			 : CvPoint & -> reference to after point 
	*			 : CvConvexityDefect * -> pointer to 1st defect point
	*			 : CvConvexityDefect * -> pointer to 2st defect point
	*			 : CvPoint & -> reference to middle point
	***/
	void differentiateDefectPoints(CvPoint &, CvPoint &, CvConvexityDefect*,  CvConvexityDefect*, CvPoint &);

	//Finger * createFinger(CvPoint, CvPoint, CvPoint, CvPoint, double, double, CvPoint);

	void findRealConvexPoint(Finger *, IplImage *);
	/***
	* extractFingerImage : extracts the ROI of the finger in the Hand image
	* parameters :	IplImage * -> the grayscale hand image.
	***/
	void extractFingerImages(IplImage *);

	/***
	* calcMidPoint = calculates the point between two points
	* parameters : CvPoint * -> 1st point
	*			 : CvPoint * -> 2nd point
	***/
	CvPoint calcMidPoint(CvPoint *, CvPoint *);
	/***
	* calcMidPoint = calculates the distance between two points
	* parameters : CvPoint -> 1st point
	*			 : CvPoint -> 2nd point
	***/
	double calcDist(CvPoint, CvPoint);
	/***
	* calcAngle = calculates angle to the origin between two points in radians 
	* parameters : CvPoint -> 1st point
	*			 : CvPoint -> 2nd point
	***/
	double calcAngle(CvPoint, CvPoint);

	int getClosestPointIndex(CvPoint, CvConvexityDefect*, CvSeq*, CvPoint, CvPoint);


	void knuckleExtraction(Finger *);

	std::vector<Finger*> getFingers()const;
};