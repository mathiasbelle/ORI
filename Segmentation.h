// Classe Segmentation, que reliza a operacao de segmentacao nos quadros

#pragma once
#include <iostream> 
#include <time.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc/segmentation.hpp>

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

class Segmentation {

public:
	Segmentation();

	// Faz a selective search no frame
	vector<Rect2d> selective_search(Mat frame, int k);
};

