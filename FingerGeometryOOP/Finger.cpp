#include "stdafx.h"
#include "Finger.h"
#include <iostream>


Finger::Finger(void)
{
}

CvPoint & Finger::rotate(CvPoint point)
{
	CvPoint temp = point;
	CvPoint newPoint;

	temp.x -= imageCenter.x;
	temp.y -= imageCenter.y;
	newPoint.x = temp.x * cos(-fingerAngle) - temp.y * sin(-fingerAngle);
	newPoint.y = temp.x * sin(-fingerAngle) + temp.y * cos(-fingerAngle);
	newPoint.x += imageCenter.x;
	newPoint.y += imageCenter.y;
	return newPoint;
}

CvPoint & Finger::calculateEndPoint(CvPoint & point)
{
	CvPoint endpoint;

	endpoint.x = point.x + ((getFingerLength() + 10) * cos(getAngle()));
	endpoint.y = point.y + ((getFingerLength() + 10) * sin(getAngle()));
	return endpoint;
}

Finger::Finger(CvPoint middle, CvPoint defect, CvPoint _before, CvPoint _after, double angle, double dist, double width, CvPoint ImageCenter):middlePoint(middle), defectPoint(defect), before(_before), after(_after), fingerAngle(angle), fingerLength(dist), fingerWidth(width), imageCenter(ImageCenter)
{
	if (width < 0)
		width *= -1;
	fingerAngleDeg = angle * (180 / 3.141592653589793238462);
	rotatedAfter = rotate(after);
	rotatedBefore = rotate(before);
	rotatedDefect = rotate(defect);
	rotatedMiddle = rotate(middle);
	endPointAfter = calculateEndPoint(after);
	endPointBefore = calculateEndPoint(before);
}

CvPoint & Finger::getBeforePointRotated()
{
	return rotatedBefore;
}

CvPoint & Finger::getAfterPointRotated()
{
	return rotatedAfter;
}

CvPoint & Finger::getMiddlePointRotated()
{
	return rotatedMiddle;
}

CvPoint & Finger::getDefectPointRotated()
{
	return rotatedDefect;
}

Finger::~Finger(void)
{
	fingerPoints.clear();
	fingerImage.release();
	baseKnuckle.release();
	middleKnuckle.release();
	tipKnuckle.release();
}

CvPoint & Finger::getBeforePoint()
{
	return before;
}

CvPoint & Finger::getAfterPoint()
{
	return after;
}

CvPoint & Finger::getMiddlePoint()
{
	return middlePoint;
}

CvPoint & Finger::getDefectPoint()
{
	return defectPoint;
}

double & Finger::getAngle()
{
	return fingerAngle;
}

double & Finger::getAngleDeg()
{
	return fingerAngleDeg;
}

double & Finger::getFingerLength()
{
	return fingerLength;
}

cv::Mat & Finger::getFingerImage()
{
	return fingerImage;
}

double & Finger::getFingerWidth()
{
	return fingerWidth;
}

CvPoint & Finger::getEndPointAfter()
{
	return endPointAfter;
}

CvPoint & Finger::getEndPointBefore()
{
	return endPointBefore;
}

cv::Mat & Finger::getBaseKnuckle()
{
	return baseKnuckle;
}

cv::Mat & Finger::getMiddleKnuckle()
{
	return middleKnuckle;
}

cv::Mat & Finger::getTipKnuckle()
{
	return tipKnuckle;
}
void Finger::setBeforePoint(CvPoint & point)
{
	before = point;
}

void Finger::setAfterPoint(CvPoint & point)
{
	after = point;
}

void Finger::setMiddlePoint(CvPoint & point)
{
	middlePoint = point;
}

void Finger::setDefectPoint(CvPoint & point)
{
	defectPoint = point;
}

void Finger::setAngle(double & angle)
{
	fingerAngle = angle;
}

void Finger::setAngleDeg(double & angle)
{
	fingerAngleDeg = angle;
}

void Finger::setFingerLength(double & dist)
{
	fingerLength = dist;
}

void Finger::setFingerImage(cv::Mat & image)
{
	fingerImage = image;
}

void Finger::setFingerWidth(double & width)
{
	fingerWidth = width;
}

std::vector<std::vector<cv::Point>> Finger::getFingerPoints()
{

	return fingerPoints;
}