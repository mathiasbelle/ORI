#pragma once
#include <iostream> 
#include <time.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/ximgproc/segmentation.hpp>
#include "opencv2/highgui.hpp"


//#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

class PreProcessing {
private:
	Mat inputFrame;
	Mat outputFrame;
public:
	PreProcessing();

	// Realiza operacoes de pre-processamento no frame
	Mat pre_process(Mat frame);
};

