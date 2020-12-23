// Classe PreProcessing, que realiza operacoes de pre-processamento nos quadros
#pragma once
#include <iostream> 
#include <opencv2/core/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


class PreProcessing {
public:
	PreProcessing();

	// Realiza operacoes de pre-processamento no frame
	Mat pre_process(Mat frame, bool lower_res);
};

