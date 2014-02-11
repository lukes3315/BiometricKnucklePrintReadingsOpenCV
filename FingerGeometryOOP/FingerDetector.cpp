#include "stdafx.h"
#include "FingerDetector.h"
#include <iostream>
#include <cv.h>
#include <highgui.h>

FingerDetector::FingerDetector(void)
{
	storage = cvCreateMemStorage(0);
}

FingerDetector::~FingerDetector(void)
{
	cvReleaseMemStorage(&storage);
}

double FingerDetector::calcDist(CvPoint p1, CvPoint p2)
{
	double dist = sqrt(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)));
	return dist;
}

double FingerDetector::calcAngle(CvPoint p1, CvPoint p2)
{
	double angle = atan2(p1.y - p2.y, p1.x - p2.x);// * (180 / 3.141592653589793238462);
	//double angle;

	
	return angle;
}

void FingerDetector::differentiateDefectPoints(CvPoint & before, CvPoint & after, CvConvexityDefect * defect1, CvConvexityDefect * defect2, CvPoint & mid)
{
	double temp = defect1->depth_point->x - defect2->depth_point->x;
	if (temp < 0)
		temp *= -1;

	if (temp > 20)
	{
		if (defect1->depth_point->x < defect2->depth_point->x)
		{
			before = *defect1->depth_point;
			after = *defect2->depth_point;
		}
		else
		{
			before = *defect2->depth_point;
			after = *defect1->depth_point;		
		}
	}
	else
	{
		if (defect1->depth_point->y < defect2->depth_point->y)
		{
			before = *defect1->depth_point;
			after = *defect2->depth_point;
		}
		else
		{
			before = *defect2->depth_point;
			after = *defect1->depth_point;		
		}
	}
}

CvPoint FingerDetector::getRealFingerTip(IplImage* image)
{
	CvPoint temp = cvPoint(0,0);

	return temp;
}

void FingerDetector::findRealConvexPoint(Finger * f, IplImage *img)
{
}

int FingerDetector::getClosestPointIndex(CvPoint p, CvConvexityDefect* d, CvSeq * defects, CvPoint bef, CvPoint aft)
{
	int index = 0;
	int min = 1000;
	for (int i = 0; i < defects->total; ++i)
	{
		double dist = calcDist(p, *d[i].start);
		if (dist < min /*&& d[i].start->x > bef.x - 100 && d[i].start->x < aft.x + 25*/)
		{
			min = dist;
			index = i;
		}
	}
	std::cout << "Index = " << index << std::endl;
	return index;
}

void FingerDetector::detectFingers(BlobDetector * blobDetector, CvSize imgSize, IplImage * img)
{
	IplImage * erodedImage = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	cvErode(img, erodedImage, 0, 10);


	CvContour * contour = 0;
	contour = (CvContour*)blobDetector->findContours(img);
	contour = (CvContour*)blobDetector->findBiggestContour((CvSeq*)contour);
	CvMemStorage * memStorageErode = cvCreateMemStorage(0);

	CvSeq * erodedImgContour = blobDetector->findContours(erodedImage);
	erodedImgContour = blobDetector->findBiggestContour(erodedImgContour);
	CvSeq * approxedContourEroded = blobDetector->approximateContour(erodedImgContour, 60, memStorageErode);
	blobDetector->calculateConvexityPoints(approxedContourEroded);
	blobDetector->calculateConvexityDefect(approxedContourEroded);
	CvConvexityDefect * defectArrayEroded = blobDetector->defectArray;
	CvSeq * defectsEroded = blobDetector->defects;


	CvSeq * approxedPoly = blobDetector->approximateContour((CvSeq*)contour, 90, storage);
	CvPoint * convexityPoints = blobDetector->calculateConvexityPoints(approxedPoly);



	blobDetector->calculateConvexityDefect(approxedPoly);
	CvConvexityDefect * defectArray = blobDetector->defectArray;
	CvSeq * defects = blobDetector->defects;

	CvPoint mid = calcMidPoint(defectArray[0].depth_point, defectArray[defects->total - 1].depth_point);
	CvPoint before;
	CvPoint after;
	differentiateDefectPoints(before, after, &defectArray[0], &defectArray[defects->total-1], mid);

	for (int i = 0 ; i < defects->total; ++i)
	{
	//	cvCircle(img, *defectArray[i].depth_point, 20, CV_RGB(255, 255, 255));
		cvCircle(img, *defectArray[i].end, 20, CV_RGB(255, 255, 255));
//		cvCircle(img, *defectArray[i].start, 20, CV_RGB(255, 255, 255));
	}




	/*for (int i = 0 ; i < ; ++i)
	{
		cvCircle(img, convexityPoints[i], 5, CV_RGB(255, 255, 255), 20);
	}*/



	int index = getClosestPointIndex(*defectArray[0].depth_point, defectArrayEroded, defectsEroded, before, after);
	CvPoint defect = *defectArrayEroded[index].end;
	double erodeMinusOriginalDist = calcDist(*defectArrayEroded[0].depth_point, *defectArray[0].depth_point);
	double angle = calcAngle(defect, mid);
	double dist = calcDist(defect, mid);
	dist += calcDist(defect, *defectArray[0].depth_point);
	double width = before.x - after.x;

	Finger * f = new Finger(mid, defect, before, after, angle, dist, width, cvPoint(imgSize.width/2, imgSize.height/2));
	cvCircle(img, mid, 10, CV_RGB(255, 0, 255), 4);
	cvCircle(img, defect, 10, CV_RGB(255, 0, 255), 4);

	fingers.push_back(f);
	for (int i = 0 ; i< defects->total; ++i)
	{
		if (i > 0 && i < defects->total - 1)
		{
			mid = calcMidPoint(defectArray[i].end, defectArray[i - 1].depth_point);
			defect = *defectArray[i].end;
			dist = calcDist(defect, mid);
			cvCircle(img, mid, 10, CV_RGB(255, 0, 255), 4);
			cvCircle(img, defect, 10, CV_RGB(255, 0, 255), 4);

		/*	std::cout << "Distance = " << dist << std::endl;
			std::cout << "count =  " << i << std::endl;
		*/	if (dist > 100 && defectArray[i].start->y < imgSize.height - 100)
			{
				index = getClosestPointIndex(*defectArray[i].depth_point, defectArrayEroded, defectsEroded, before, after);
				dist = calcDist(defect, *defectArrayEroded[index].depth_point) + calcDist(defect, mid);
				defect = *defectArrayEroded[index].depth_point;
				differentiateDefectPoints(before, after, &defectArray[i], &defectArray[i - 1], mid);
				angle = calcAngle(defect, mid);
				width = before.x - after.x;
				f = new Finger(mid, defect, before, after, angle, dist, width, cvPoint(imgSize.width/2, imgSize.height/2));
				fingers.push_back(f);
			}
		}
	}
	mid = calcMidPoint(defectArray[defects->total - 1].depth_point, defectArray[defects->total - 2].depth_point);
	index = getClosestPointIndex(*defectArray[defects->total - 1].depth_point, defectArrayEroded, defectsEroded, before, after);
	defect = *defectArrayEroded[index].depth_point;
	differentiateDefectPoints(before, after, &defectArray[defects->total - 1], &defectArray[defects->total - 2], mid);
	angle = calcAngle(defect, mid);
	cvCircle(img, mid, 10, CV_RGB(255, 0, 255), 4);
	cvCircle(img, defect, 10, CV_RGB(255, 0, 255), 4);

	dist = calcDist(*defectArray[defects->total-1].depth_point, mid);
	erodeMinusOriginalDist = calcDist(defect, *defectArray[defects->total - 1].depth_point);
	dist += erodeMinusOriginalDist;
	width = before.x - after.x;
	f = new Finger(mid, defect, before, after, angle, dist, width, cvPoint(imgSize.width/2, imgSize.height/2));
	fingers.push_back(f);

	index = getMaxIndex();
	removeAtIndex(index);
	index = getMinIndex();
	removeAtIndex(index);
	

	std::cout << "fingerarray size = " << fingers.size() << " Defects = "  <<defects->total << std::endl;

	cvDrawContours(img, approxedPoly, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1, 5);

	free(convexityPoints);
	cvReleaseImage(&erodedImage);
}

void FingerDetector::knuckleExtraction(Finger *f)
{
	double v = f->getFingerLength() / 3;
	double w = f->getFingerWidth();

	cv::Rect r1 = cv::Rect(0, 0, v + 20, w);
	cv::Rect r2 = cv::Rect(v - 50, 0, v + 20, w);
	cv::Rect r3 = cv::Rect((v * 2) - 50, 0, v + 20, w);

	f->baseKnuckle = f->getFingerImage()(r1);
	f->middleKnuckle = f->getFingerImage()(r2);
	f->tipKnuckle = f->getFingerImage()(r3);
}

void FingerDetector::extractFingerImages(IplImage * image)
{
	for (int j = 0; j < fingers.size() ; ++j)
	{
		cv::Mat matTemp(image);
		cv::Point2f center(matTemp.cols/2, matTemp.rows/2);
		cv::Mat rotation = cv::getRotationMatrix2D(center, fingers[j]->getAngleDeg(), 1.0);
		cv::Mat dest;
		cv::warpAffine(matTemp, dest, rotation, matTemp.size());
		int tempX = 0;
		int tempY = 0;
		double width = fingers[j]->getBeforePointRotated().y - fingers[j]->getAfterPointRotated().y;

		if (width < 0)
			width *= -1;
		width += 50;

		double x = fingers[j]->getBeforePoint().x;


		if (fingers[j]->getBeforePoint().y > fingers[j]->getAfterPoint().y)
		{
			x = fingers[j]->getAfterPointRotated().x;
		}
		if (fingers[j]->getBeforePoint().y < fingers[j]->getAfterPoint().y)
		{
			x = fingers[j]->getBeforePointRotated().x;
		}

		int len = fingers[j]->getFingerLength() + 100;
		if ((x - 5 + fingers[j]->getFingerLength() + 100) > dest.cols)
		{
			int newLen = (x-5 + len) - dest.cols;
			len -= newLen + 1;
		}
		cv::Rect Roi = cv::Rect(x - 30, fingers[j]->getBeforePointRotated().y - 20, len, width);
		cv::Mat imgRoi = dest(Roi);

		std::vector<std::vector<std::vector<cv::Point>>> vectors;
		int i = 0;
		bool b = false;

		for (int x = 50 ; x < imgRoi.cols; ++x)
		{
			std::vector<std::vector<cv::Point>> vectorY;	
			b = false;
			std::vector<cv::Point> points;
			for (int y = 0 ; y < imgRoi.rows; ++y)
			{
				if (imgRoi.at<uchar>(y, x) > 0)
				{
					points.push_back(cv::Point(x, y));

					b = true;
				}
				else
				{
					if (b == true)
					{
						vectorY.push_back(points);
						points.clear();
					}
					b = false;
				}
			}
			vectors.push_back(vectorY);
		}
		int maxDistImage = 0;
		cv::Point thePoint1, thePoint2;



		for (std::vector<std::vector<std::vector<cv::Point>>>::iterator it = vectors.begin(); it != vectors.end(); ++it)
		{
			int maxDist = 0;
			cv::Point p1, p2;
			for (std::vector<std::vector<cv::Point>>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
			{
				std::vector<cv::Point> v = *it2;
				int dist = calcDist(v[0], v[v.size()-1]);
				if (dist > maxDist)
				{
					p1 = v[0];
					p2 = v[v.size()-1];
					maxDist = dist;
				}
			}

			if (maxDist > maxDistImage)
			{
				maxDistImage = maxDist;
				thePoint1 = p1;
				thePoint2 = p2;
			}
		}

		thePoint1.y -= 10;
		thePoint2.y += 10;
		if (thePoint1.y < 0)
			thePoint1.y = 0;


		double newWidth = thePoint2.y - thePoint1.y;
		fingers[j]->setFingerWidth(newWidth);
		cv::Rect roi = cv::Rect(0, thePoint1.y, fingers[j]->getFingerLength() + 30, newWidth);

		cv::Mat imgRoi2 = imgRoi(roi);


		fingers[j]->setFingerImage(imgRoi2);

		for (int x = 0; x < imgRoi2.cols; ++x)
		{
			std::vector<cv::Point> pts;
			for (int y = 0 ; y < imgRoi2.rows; ++y)
			{
				if (imgRoi2.at<uchar>(y, x) > 0)
				{
					pts.push_back(cv::Point(x, y));
				}
			}
			fingers[j]->fingerPoints.push_back(pts);
			pts.clear();
		}
		
		double max = 0;

		for (int y = 0 ; y < imgRoi2.rows ; ++y)
		{
			for (int x = 0 ; x < imgRoi2.cols ;++x)
			{
				if (imgRoi2.at<uchar>(y, x) > 0)
				{
					if (x > max)
					{
						max = x;
					}
				}
			}
		}



		fingers[j]->setFingerLength(max);



		roi = cv::Rect(0, 0, fingers[j]->getFingerLength() + 2, newWidth);


		imgRoi2 = imgRoi2(roi);
		fingers[j]->setFingerImage(imgRoi2);

		knuckleExtraction(fingers[j]);
		 
		matTemp.release();
		rotation.release();
		dest.release(); 
		imgRoi.release();
		imgRoi2.release();
		//imgRoi3.release();

		for (std::vector<std::vector<std::vector<cv::Point>>>::iterator it = vectors.begin() ; it != vectors.end() ; ++it)
		{
			for (std::vector<std::vector<cv::Point>>::iterator it2 = it->begin() ; it2 != it->end() ; ++it2)
			{
				it2->clear();
			}
			it->clear();
		}
		vectors.clear();
	}
}

CvPoint FingerDetector::calcMidPoint(CvPoint *p1, CvPoint *p2)
{
	CvPoint mid;
	mid.x = (p1->x + p2->x)/2;
	mid.y = (p1->y + p2->y)/2;
	return mid;
}

std::vector<Finger*> FingerDetector::getFingers()const
{
	return fingers;
}

int FingerDetector::getMinIndex()
{
	CvPoint min = cvPoint(10000, 10000);
	int index = 0;
	
	for (int i = 0; i < fingers.size() ; ++i)
	{
		CvPoint mid;

		mid = fingers[i]->getMiddlePoint();

		if (mid.x < min.x)
		{
			index = i;
			min = mid;
		}
	}
	return index;
}

int FingerDetector::getMaxIndex()
{
	CvPoint max = cvPoint(0, 0);
	int index = 0;

	for (int i = 0; i < fingers.size() ; ++i)
	{
		CvPoint mid;

		mid = fingers[i]->getMiddlePoint();

		if (mid.x > max.x)
		{
			 index = i;
			 max = mid;
		}
	}
	return index;
}

void FingerDetector::removeAtIndex(int index)
{
	int i = 0;

	if (index >= 0 && index < fingers.size())
	{
		for (std::vector<Finger*>::iterator it = fingers.begin() ; it != fingers.end() ; ++it)
		{
			if (i == index)
			{
				it = fingers.erase(it);
			}
			++i;
		}
	}
}