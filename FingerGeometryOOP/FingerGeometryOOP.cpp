// FingerGeometryOOP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <iostream>
#include <crtdbg.h>
#include "BlobDetector.h"
#include "FilterManager.h"
#include "FingerDetector.h"

#define _CRTDBG_MAP_ALLOC


class binaryTree
{
public :
	binaryTree()
	{}
	~binaryTree()
	{}
};

struct Node
{
	cv::Point pt;
	std::vector<Node*> nodes;
	Node * prevNode;
};

template<typename T>
void removeLine(cv::Mat & img, int x)
{
	if (x < img.cols)
	{
		for (int y = 0 ; y < img.rows; ++y)
		{
			img.at<T>(y, x) = 0;
		}
	}
}

template<typename T>
void compareLine(cv::Mat & img1, cv::Mat & img2, int x)
{
	if (x < img2.cols)
	{
		for (int x1 = 0 ; x1 < img2.cols ; ++x1)
		{
			bool b = true;
			for (int y = 0 ; y < img2.rows; ++y)
			{
				if (img2.at<T>(y, x1) != img1.at<T>(y, x))
				{
					b = false;
				}
			}
			if (b)
				removeLine<T>(img1, x);
		}
	}
}

template<typename T>
void compareImages(cv::Mat & img1, cv::Mat & img2)
{
	for (int x = 0 ; x < img1.cols ; ++x)
	{
		compareLine<T>(img1, img2, x);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage * img = cvLoadImage("004.jpg", 0);	
	FilterManager * filterManager = new FilterManager();
	BlobDetector blobDetector;
	FingerDetector fingerDetector;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	IplImage * imgResize1 = filterManager->resize(img, cvSize(img->width/2, img->height/2));
	IplImage * imgThresh = filterManager->threshold(imgResize1);
	CvSeq * contours = blobDetector.findContours(imgThresh);
	CvSeq * biggestContour = blobDetector.findBiggestContour(contours);
	cvZero(imgThresh);
	cvDrawContours(imgThresh, biggestContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1, CV_FILLED);
	IplImage * combined = filterManager->combineImages(imgThresh, imgResize1);

	cvMoveWindow("combined", 0, 0);
	cvNamedWindow("combined");
	cvNamedWindow("wind1");
	fingerDetector.detectFingers(&blobDetector, cvSize(combined->width, combined->height), combined);

	cvMoveWindow("combined", 0, 0);

	std::vector<Finger*> fingers = fingerDetector.getFingers();

	std::cout << "here" << std::endl;
	//cvCanny(combined, combined, 50, 50, 3);
	//combined = filterManager->smooth(combined, CV_GAUSSIAN, 31, 31, 30, 30);
	//combined = filterManager->threshold(combined, 40, 255, CV_THRESH_BINARY_INV);

	//IplImage * temp = cvCreateImage(cvSize(combined->width, combined->height), 8, 1);
	//IplConvKernel * kernel = cvCreateStructuringElementEx(7, 7, 3, 3, CV_SHAPE_RECT);
	//cvMorphologyEx(combined, combined, temp, kernel, CV_MOP_GRADIENT);
	//cvShowImage("t", combined);


	IplImage * combined2 = cvCreateImage(cvSize(combined->width, combined->height), 8, 1);
	cvCopy(combined, combined2);

	///combined = (IplImage*)cv2DRotationMatrix(cvPoint2D32f(0, 0), 180, 5, (CvMat*)combined);
	///*IplImage * eigen = cvCreateImage(cvSize(combined->width * 2, combined->height), CV_32F, 3);
	//IplImage * combinedtemp = cvCreateImage(cvSize(combined->width, combined->height), CV_32F, 1);
	//cvCopy(combined, combinedtemp);
	//cvCornerEigenValsAndVecs(combinedtemp, eigen, 10);
	//*/cvNamedWindow("eigen");
	//cvShowImage("eigen", eigen);

	//for (int i = 0; i < fingers.size(); ++i)
	//{
	//	cvCircle(combined, fingers[i]->getAfterPoint(), 10, CV_RGB(255, 255, 0), 4);
	//	cvCircle(combined, fingers[i]->getBeforePoint(), 10, CV_RGB(255, 255, 255), 4);
	//	cvCircle(combined, fingers[i]->getMiddlePoint(), 5, CV_RGB(255, 255, 255), 4);
	//	cvCircle(combined, fingers[i]->getDefectPoint(), 5, CV_RGB(255, 255, 255), 4);

	//	cvLine(combined, fingers[i]->getAfterPoint(), fingers[i]->getMiddlePoint(), CV_RGB(255, 255, 255), 3);
	//	cvLine(combined, fingers[i]->getBeforePoint(), fingers[i]->getMiddlePoint(), CV_RGB(255, 255, 255), 3);
	//	cvLine(combined, fingers[i]->getDefectPoint(), fingers[i]->getMiddlePoint(), CV_RGB(255, 255, 255), 3);
	//	cvLine(combined, fingers[i]->getBeforePoint(), fingers[i]->getDefectPoint(), CV_RGB(255, 255, 255), 3);
	//	cvLine(combined, fingers[i]->getAfterPoint(), fingers[i]->getDefectPoint(), CV_RGB(255, 255, 255), 3);

	//	//double n = fingers[i]->getAfterPoint().y - (tan(fingers[i]->getAngle() * fingers[i]->getBeforePoint().x);
	//	CvPoint endPoint, endPoint2;
	//	endPoint = fingers[i]->getEndPointAfter();
	//	endPoint2 = fingers[i]->getEndPointBefore();

	//	cvLine(combined, fingers[i]->getAfterPoint(), endPoint, CV_RGB(255, 255, 255), 1);


	//	std::cout << i << " : " << fingers[i]->getFingerLength() << std::endl;

	//	cvLine(combined, fingers[i]->getBeforePoint(), endPoint2, CV_RGB(255, 255, 255), 1);
	//	cvLine(combined, endPoint, endPoint2, CV_RGB(255, 255, 255), 1);
	//}

	cvShowImage("combined", combined);
	std::cout << "here1" << std::endl;
	cvWaitKey(0);
	//IplImage * imgThresTemp = cvCreateImage(cvSize(imgThresh->width, imgThresh->height), 8, 1);
	//cvCopy(imgThresh, imgThresTemp);
	//cvErode(imgThresTemp, imgThresTemp, 0, 20);

	//CvContour * contours1 = 0;
	//contours1 = (CvContour*)blobDetector.findContours(imgThresTemp);
	//contours1 = (CvContour*)blobDetector.findBiggestContour((CvSeq*)contours1);
	//CvMemStorage * meme = cvCreateMemStorage(0);
	//CvSeq * approxPoly = cvApproxPoly(contours1, sizeof(CvContour), meme, CV_POLY_APPROX_DP, 60);
	//cvDrawContours(imgThresTemp, approxPoly, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1);

	//CvPoint * points = (CvPoint *) malloc(sizeof(CvPoint) * approxPoly->total);
	//

	cvShowImage("wind1", imgThresh);

	//std::vector<IplImage *> fingerImages;
			std::cout << "in here 1" << std::endl;

	fingerDetector.extractFingerImages(combined2);
		std::cout << "in here 1" << std::endl;

	fingers = fingerDetector.getFingers();


			std::cout << "here3" << std::endl;

	//IplConvKernel * kernel = cvCreateStructuringElementEx(7, 7, 3, 3, CV_SHAPE_RECT);


	///*::Mat kern = */cv::createMorphologyFilter(cv::MORPH_ERODE, CV_8U, kern);
	cv::Mat neuron(640, 640, CV_8U, 1);
	if (cvLoadImage("neuron.bmp") != NULL)
	{
		cv::Mat neuronTemp = cv::imread("neuron.bmp");
		cv::cvtColor(neuronTemp, neuron, CV_BGR2GRAY);
	}
	else
	{
		for (int i = 0; i < 640 * 640 ; ++i)
		{
			neuron.data[i] = 0;
		}
	}


	cv::Mat kern;
	cv::Mat * temp = new cv::Mat(3, 3, CV_8U);
	for (int i = 0 ; i < temp->rows * temp->cols ; ++i)
	{
		temp->data[i] = 0;
	}
#define MORPHVALUE 10
	temp->data[0] = MORPHVALUE;
	temp->data[3] = MORPHVALUE;
	temp->data[6] = MORPHVALUE;
	temp->data[2] = MORPHVALUE;
	temp->data[5] = MORPHVALUE;
	temp->data[8] = MORPHVALUE;
	temp->data[(2) * 2] = -10;
	cv::createMorphologyFilter(cv::MORPH_DILATE, CV_8U, *temp);
	CvMemStorage * mem = cvCreateMemStorage(0);

	//for (int k = 0; k < 10; ++k)
	//{


	int kernel_size=15;
    int pos_sigma= 1;
    int pos_lm = 0;
    int pos_th = 0;
    int pos_psi = 90;
    cv::Mat Dest;
    double sig = pos_sigma;

	for (int j = 0; j < fingers.size() ; ++j)
	{
		cv::Mat  img = fingers[j]->getFingerImage();
		cv::Mat * imgCpy = new cv::Mat(img.rows, img.cols, img.type());


		std::cout << "in here 2" << std::endl;
		img.copyTo(*imgCpy);
		std::cout << "in here 3" << std::endl;

		//cv::erode(*imgCpy, *imgCpy, *temp, cv::Point(0, 0), 5);
		//cv::erode(img, img, *temp, cv::Point(0, 0), 5);
		//
		//cv::Sobel(*imgCpy, *imgCpy, imgCpy->depth(), 1, 1, 3);
		//cv::Sobel(img, img, imgCpy->depth(), 1, 1, 3);

		//cv::Canny(img, *imgCpy, 40, 200);


		//cv::threshold(*imgCpy, *imgCpy, 1, 255, CV_THRESH_BINARY);

		//
		//


	//	cvThreshold(temp, temp, 12, 255, CV_THRESH_BINARY);0
//		temp = filterManager->smooth(temp,CV_GAUSSIAN,3, 3, 2,2 );


        //cv::imshow("Kernel", Lkernel);
	


		/*int x0 = 50;//Draw a circle prog
		int y0 = 50;
		int radius = 50;

		int x = radius;
		int y = 0;
		int radiusError = 1 -x;



		while(x >= y)
		{
			img.at<uchar>(x + x0, y + y0) = 255;
			img.at<uchar>(y + x0, x + y0)= 255;
			img.at<uchar>(-x + x0, y + y0)= 255;
			img.at<uchar>(-y + x0, x + y0)= 255;
			img.at<uchar>(-x + x0, -y + y0)= 255;
			img.at<uchar>(-y + x0, -x + y0)= 255;
			img.at<uchar>(x + x0, -y + y0)= 255;
			img.at<uchar>(y + x0, -x + y0)= 255;

			y++;
			if(radiusError<0)
				radiusError+=2*y+1;
			else
			{
				x--;
				radiusError+=2*(y-x+1);
			}
		}
*///draw circle
		

		std::cout << "in here " << std::endl;

		cv::Mat roi1 = fingers[j]->getBaseKnuckle();
		cv::Mat roi2 = fingers[j]->getMiddleKnuckle();
		cv::Mat roi3 = fingers[j]->getTipKnuckle();
	std::cout << "in here " << std::endl;
		//cv::Mat roi1G;
		//cv::Mat roi2G;
		//cv::Mat roi3G;

		//double lm = 0;
		//cv::Mat kernel = filterManager->createGaborKernel(kernel_size, sig, pos_th, pos_lm, pos_psi);
		//cv::filter2D(img, Dest, CV_32F, kernel);

		//cv::filter2D(roi1, roi1G, CV_32F, kernel);
		//cv::filter2D(roi2, roi2G, CV_32F, kernel);
		//cv::filter2D(roi3, roi3G, CV_32F, kernel);
		//cv::Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
		//cv::resize(kernel, Lkernel, Lkernel.size());
		//Lkernel /= 2.;
		//Lkernel += 0.5;
		//cv::Mat mag, mag1, mag2, mag3;
		//cv::pow(Dest, 2, mag);
		//cv::pow(roi1G, 2, mag1);
		//cv::pow(roi2G, 2, mag2);
		//cv::pow(roi3G, 2, mag3);

		

		cv::imshow("mag1", roi1);
		cv::imshow("mag2", roi2);
		cv::imshow("mag3", roi3);

		//IplImage* image2=cvCloneImage(&(IplImage)img);
		//cvSaveImage(nameImg.c_str(), image2);
		/*cv::Mat imgSrc = cv::imread(nameImg);


		cv::Mat srcGray(imgSrc.cols, imgSrc.rows, roi2.type());
		cv::cvtColor(imgSrc, srcGray, CV_RGB2GRAY);
*/
		//std::cout << imgSrc.channels() << " " << roi2.channels() << std::endl; 
		//cv::imshow("sourceimagedsaved", imgSrc);
		cv::Mat res(640, 480, CV_32FC1);
		int match_method = CV_TM_CCOEFF_NORMED;
		//cv::matchTemplate(srcGray, roi2, res, CV_TM_CCOEFF_NORMED);
		//cv::normalize( res, res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
		//double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
		//cv::Point matchLoc;
		//minMaxLoc( res, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
		//if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
		//{
		//	matchLoc = minLoc; 
		//}
		//else
		//{ matchLoc = maxLoc; }

		//rectangle( img, matchLoc, cv::Point( matchLoc.x + roi3.cols , matchLoc.y + roi3.rows ), cv::Scalar::all(0), 2, 8, 0 );
		//rectangle( res, matchLoc, cv::Point( matchLoc.x + roi3.cols , matchLoc.y + roi3.rows ), cv::Scalar::all(0), 2, 8, 0 );


//		cv::imshow("res", res);

		cv::Mat m(roi2.rows, roi2.cols, roi2.type());
		roi2.copyTo(m);

		compareImages<uchar>(roi1, roi2);

		cv::Mat m2(m.rows, m.cols, m.type());
		m.copyTo(m2);
		compareImages<uchar>(m, roi3);
		compareImages<uchar>(roi3, m2);

		cv::Sobel(roi1, roi1, roi1.depth(), 1, 0, 3);
		cv::threshold(roi1, roi1, 5, 255, CV_THRESH_BINARY);
		cv::imshow("r1", roi1);
		cv::imshow("r2", m2);
		cv::imshow("r3", roi3);

        //cv::imshow("Mag", mag);

		//cv::imwrite("finger.bmp", mag);

		cv::imshow("Process window", Dest);


//		cv::line(img, cv::Point(0, mag.rows/2), cv::Point(fingers[j]->getFingerLength() / 3, mag.rows/2), CV_RGB(255, 255, 255), 5);
//		cv::line(img, cv::Point(v, mag.rows/2), cv::Point(v * 2, mag.rows/2), CV_RGB(255, 255, 255), 5);
        


		//fingers[j]->getFingerLength();





//		std::vector<std::vector<cv::Point>> fingerPixels = fingers[j]->getFingerPoints();
//		float value = 0.0;
//		float count = 0.0;
//
//
//
//		for (std::vector<std::vector<cv::Point>>::iterator it = fingerPixels.begin() ; it != fingerPixels.end() ; ++it)
//		{
//
//			for (std::vector<cv::Point>::iterator it2 = it->begin() ; it2 != it->end() ; ++it2)
//			{
//				if (mag.at<float>(it2->y, it2->x) > 0)
//				{
//					value += mag.at<float>(it2->y, it2->x);
//				}
//				++count;
//			}
//			++count;
//		}
//		value /= count;
//		
//		for (std::vector<std::vector<cv::Point>>::iterator it = fingerPixels.begin() ; it != fingerPixels.end() ; ++it)
//		{
//			count = 0;
//			for (std::vector<cv::Point>::iterator it2 = it->begin() ; it2 != it->end() ; ++it2)
//			{
//				if (mag.at<float>(it2->y, it2->x) > value)
//				{
//					++count;
//				}
//			}
//			if (count > it->size() - it->size() / 2)
//			{
//				cv::line(img, it->at(0), it->at(it->size()-1), CV_RGB(255, 255, 255), 1);
//			}
//		}
//
//
//std::vector<int> countingDensity;
//std::vector<cv::Mat> densityMatrix;
//
//
//		std::cout << "value = " << value << std::endl;
//		for (int i = 0 ; i < img.cols - 51; i+=50)
//		{
//
//			cv::Rect r = cv::Rect(i, 0, 50, fingers[j]->getFingerWidth());
//			mag.copyTo(*imgCpy);
//			cv::Mat ROI = (*imgCpy)(r);
//			int count2 = 0;
//			int counttotal = 0;
//			for (int x = 0 ; x < ROI.cols ; ++x)
//			{
//				for (int y = 0 ; y < ROI.rows ; ++y)
//				{
//					if (ROI.at<float>(y, x) > 0.1)
//					{
//						++count2;
//						//ROI.at<float>(y, x) = 0.9;
//					}
//				}
//			}
//
//			//std::cout << "number = " << count2 << " total = " << ROI.rows * ROI.cols<< std::endl;
//			double percentage = (double)count2 / (double)(ROI.rows * ROI.cols);
//			percentage *= 100;
//			
//			std::cout << percentage << std::endl;
//			if (percentage > 20)
//			{
//
//				cv::rectangle(img, r, CV_RGB(255, 255, 255), 1);			
//				//sdensityMatrix.push_back(ROI);
//			}
//
//			//cv::threshold(ROI, ROI, 150, 255, CV_THRESH_BINARY_INV);
//			//cv::Canny(ROI, ROI, 1, 50, 5);
//			
//		}
//		for (int i = 0 ; i < densityMatrix.size() ; ++i)
//		{
//
//			//cv::imshow("density" + i, densityMatrix[i]);
//		}
		cv::imshow("temp", img);
		cv::imshow("t", *imgCpy);
		cv::waitKey(0);


		//cv::GaussianBlur(img, img, cv::Size(15, 15), 0, 0, 32);
		//cv::Canny(img, img, 1, 10, 7);
		//cv::morphologyEx(img, img, cv::MORPH_ELLIPSE, *temp, cv::Point(2, 2), 10, 0);
		//for (int i = 0; i < img.rows * img.cols ; ++i)
		//{
		//	if (img.data[i] > 0)
		//	{
		//		img.data[i] = 255;
		//	}
		//}
		//for (int x = 0; x < img.rows;++x)
		//{
		//	for (int y = 0; y < img.cols; ++y)
		//	{
		//		if (img.at<uchar>(x, y) > 0)
		//		{
		//			if (neuron.at<uchar>(x, y) < 245)
		//				neuron.at<uchar>(x, y) += 10;
		//		}
		//		else if (img.at<uchar>(x, y) == 0)
		//		{
		//			if (neuron.at<uchar>(x, y) >= 10)
		//				neuron.at<uchar>(x, y) -= 10;

		//		}
		//	}
		//}

		imgCpy->release();
		roi1.release();
		roi2.release();
		roi3.release();
		//mag.release();

	}
	Dest.release();
//	}
	//		cv::imshow("finger", img);
	//cv::waitKey(0);
	//cv::Mat m(neuron);
	//cv::GaussianBlur(m, m, cv::Size(5, 5), 3);
	//cv::threshold(m, m, 20, 255, CV_THRESH_BINARY);
	//cv::dilate(m, m, *temp, cv::Point(2, 2), 3);
	//cv::imshow("neuron", m);\


	try
	{
	cv::imwrite("neuron.bmp", neuron);
	}
	catch (std::exception e)
	{
	std::cout << e.what() << std::endl;
	}
	//std::cout << "wtf3" << std::endl;
	cvSaveImage("temp.jpg", combined);
	cvReleaseImage(&img);
	cvReleaseImage(&imgResize1);
	_CrtDumpMemoryLeaks();
/*std::string lol = "";
std::cin >> lol;
*/

	return 0;
}

