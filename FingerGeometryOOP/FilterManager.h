#pragma once
#include <cv.h>
#include <highgui.h>

class FilterManager
{
public:
	FilterManager(void);
	~FilterManager(void);

	/***
	* resize : resize image to given width and height
	* parameters : IplImage * -> input image to resize
	*			 : CvSize -> size to change to
	***/
	IplImage * resize(IplImage *, CvSize);//Resize image
	/***
	* threshold : given a grayscale image, returns a thresholded image
	* parameters : IplImage * -> input image to threshold
	*			 : int -> min threshold
	*			 : int -> max threshold
	*			 : int -> threshold type (CV_THRESH_BINARY/CV_THRESH_BINARY_INV)
	***/
	IplImage * threshold(IplImage *, int = 49, int = 255, int = CV_THRESH_BINARY);//Threshold image (image must be grayscale)

	/***
	* smooth : apply smoothing filter (Gaussian, etc...)
	* parameters : IplImage * -> input image to transform
	* 		   : int -> filter type
	*		   : int -> x kernel size
	*		   : int -> y kernel size
	*		   : float -> sigma X
	*		   : float -> sigma Y
	***/
	IplImage * smooth(IplImage *, int = CV_GAUSSIAN, int = 1, int = 1, float = 0.0, float = 0.0);
	
	/***
	* combineImages : one image is binary thresholded and one is grayscale 
	* it will write the grayscale image onto the binary image
	* parameters : IplImage * -> thresholded image
	*			 : IplImage * -> grayscale image
	***/
	IplImage * combineImages(IplImage *, IplImage *);

	/***
	* createGaborKernel : generates a gabor kernel to apply to an 8bit grayscale image
	* parameters : int ks -> size of the kernel
	*			 : double -> sigma 
	*			 : double -> theta
	*			 : double -> lamba
	*			 : double -> phase offset
	***/
	cv::Mat createGaborKernel(int, double, double, double, double);
};

