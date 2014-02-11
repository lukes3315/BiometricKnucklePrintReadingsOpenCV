#pragma once
#include <cv.h>


class Finger
{
	friend class FingerDetector;
private :
	CvPoint before;
	CvPoint after;
	CvPoint middlePoint;
	CvPoint defectPoint;
	double fingerAngle;
	double fingerAngleDeg;
	double fingerLength;
	double fingerWidth;

	CvPoint rotatedBefore;
	CvPoint rotatedAfter;
	CvPoint rotatedMiddle;
	CvPoint rotatedDefect;


	CvPoint imageCenter;

	cv::Mat fingerImage;

	CvPoint endPointBefore;
	CvPoint endPointAfter;

	cv::Mat baseKnuckle;
	cv::Mat middleKnuckle;
	cv::Mat tipKnuckle;



	std::vector<std::vector<cv::Point>> fingerPoints;

	CvPoint & rotate(CvPoint);

	CvPoint & calculateEndPoint(CvPoint &);

	void setBeforePoint(CvPoint &);
	void setAfterPoint(CvPoint &);
	void setMiddlePoint(CvPoint &);
	void setDefectPoint(CvPoint &);
	void setAngle(double &);
	void setAngleDeg(double &);
	void setFingerLength(double &);
	void setFingerWidth(double &);
public :
	Finger(void);
	Finger(CvPoint, CvPoint, CvPoint, CvPoint, double, double, double, CvPoint);
	~Finger(void);

	void setFingerImage(cv::Mat &);

	CvPoint & getBeforePoint();
	CvPoint & getAfterPoint();
	CvPoint & getMiddlePoint();
	CvPoint & getDefectPoint();

	CvPoint & getBeforePointRotated();
	CvPoint & getAfterPointRotated();
	CvPoint & getMiddlePointRotated();
	CvPoint & getDefectPointRotated();
	
	double & getAngle();
	double & getAngleDeg();
	double & getFingerLength();
	cv::Mat & getFingerImage();
	double & getFingerWidth();

	CvPoint & getEndPointBefore();
	CvPoint & getEndPointAfter();

	cv::Mat & getBaseKnuckle();
	cv::Mat & getMiddleKnuckle();
	cv::Mat & getTipKnuckle();

	std::vector<std::vector<cv::Point>> getFingerPoints();
};

