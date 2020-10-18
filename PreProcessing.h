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
	PreProcessing() {
		//cvtColor(frame, inputFrame, COLOR_BGR2GRAY);
		//Mat crop_rect(inputFrame, Rect(35, 90, 1180, 630));
		//Mat cropped;
		//crop_rect.copyTo(cropped);
		//imwrite("cropped.png", cropped);
		////imwrite("img.png", inputFrame);
		//Mat blurredFrame;
		////GaussianBlur(inputFrame, blurredFrame, Size(11, 11), 0, 0);
		//medianBlur(cropped, blurredFrame, 11);
		//Mat thresholdFrame;
		//adaptiveThreshold(blurredFrame, thresholdFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY ,11, 2);
		//imwrite("img_gauss.png", thresholdFrame);
	}

	//Mat edge_detectors(Mat blurredFrame) {
	//	Mat thresholdFrame;
	//	// CANNY
	//	//Canny(blurredFrame, thresholdFrame, 20, 60, 3);
	//	// LAPLACIAN
	//	//Laplacian(blurredFrame, thresholdFrame, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//	// SOBEL
	//	//Mat grad_x, grad_y;
	//	//Mat abs_grad_x, abs_grad_y;
	//	//Sobel(blurredFrame, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	//	//Sobel(blurredFrame, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	//	////// converting back to CV_8U
	//	//convertScaleAbs(grad_x, abs_grad_x);
	//	//convertScaleAbs(grad_y, abs_grad_y);
	//	//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, thresholdFrame);

	//}

	// Apply the Selective Search algorithm on the input frame
	// Return the frame with squares on the detected objects
	//vector<Rect> selective_search(Mat frame) {
	//	
	//	clock_t start, end;
	//	start = clock();
	//	Mat crop_rect(frame, Rect(35, 90, 1180, 630));
	//	crop_rect.copyTo(frame);
	//	medianBlur(frame, frame, 7);
	//	// speed-up using multithreads
	//	//setUseOptimized(true);
	//	//setNumThreads(4);

	//	// read image
	//	// resize image
	//	//int newHeight = 200;
	//	//int newWidth = frame.cols * newHeight / frame.rows;
	//	//resize(frame, frame, Size(newWidth, newHeight));

	//	

	//	// Creates selective search
	//	Ptr<SelectiveSearchSegmentation> selective_search = createSelectiveSearchSegmentation();
	//	// Set input image
	//	selective_search->setBaseImage(frame);
	//	// Select single strategy, with k = 850, its faster, and produces less, but more important, results
	//	selective_search->switchToSingleStrategy(850);
	//	
	//	// creates a Rect vector to store the ractangles that selective search finds
	//	vector<Rect> rects;
	//	selective_search->process(rects);
	//	std::cout << "Numero de regioes: " << rects.size() << std::endl;
	//	// Sorts the vector, to have the rects with the biggest area first
	//	sort(rects.begin(), rects.end(), [](const Rect& left, const Rect& right) {
	//		return left.area() > right.area();
	//		}
	//	);
	//	// Removes the rect that encompasses the whole image
	//	rects.erase(rects.begin());

	//	Mat imOut = frame.clone();

	//	// itereate over all the region proposals
	//	for (int i = 0; i < rects.size(); i++) {
	//		rectangle(imOut, rects[i], Scalar(0, 255, 0));
	//	}
	//	imwrite("imagem.png", imOut);
	//	end = clock();
	//	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	//	//cout << "Tempo da segmentacao: " << fixed << time_taken << setprecision(5) << endl;
	//	return rects;
	//}

	Mat pre_process(Mat frame) {
		//cvtColor(frame, inputFrame, COLOR_BGR2GRAY);
		Mat crop_rect(frame, Rect(35, 90, 1180, 630));
		Mat cropped;
		crop_rect.copyTo(cropped);

		// ========================================== LIMIARIZACAO ORIGINAL
		imwrite("orignal.png", frame);
		imwrite("cropped.png", cropped);
		//imwrite("img.png", inputFrame);
		Mat blurred_frame;
		//GaussianBlur(inputFrame, blurredFrame, Size(11, 11), 0, 0);
		//medianBlur(cropped, blurred_frame, 7);
		blur(cropped, blurred_frame, Size(3, 3));
		
		//imwrite("blurred_frame.png", blurred_frame);

		//return cropped;
		return blurred_frame;
		//Mat thresholdFrame;
		// CANNY
		//Mat kernel_laplace = (Mat_<float>(3, 3) <<
		//	1, 1, 1,
		//	1, -8, 1,
		//	1, 1, 1); // an approximation of second derivative, a quite strong kernel
		//	// do the laplacian filtering as it is
		//	// well, we need to convert everything in something more deeper then CV_8U
		//	// because the kernel has some negative values,
		//	// and we can expect in general to have a Laplacian image with negative values
		//	// BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
		//	// so the possible negative number will be truncated
		//Mat imgLaplacian;
		//filter2D(blurredFrame, imgLaplacian, CV_32F, kernel_laplace);
		//Mat sharp;
		//blurredFrame.convertTo(sharp, CV_32F);
		//Mat imgResult = sharp - imgLaplacian;
		//// convert back to 8bits gray scale
		//imgResult.convertTo(imgResult, CV_8U);
		//Canny(imgResult, thresholdFrame, 100, 200, 3);
		//Canny(blurredFrame, thresholdFrame, 20, 60, 3);
		// LAPLACIAN
		//Laplacian(blurredFrame, thresholdFrame, CV_16S, 3, 1, 0, BORDER_DEFAULT);
		// SOBEL
		//Mat grad_x, grad_y;
		//Mat abs_grad_x, abs_grad_y;
		//Sobel(blurredFrame, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		//Sobel(blurredFrame, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
		////// converting back to CV_8U
		//convertScaleAbs(grad_x, abs_grad_x);
		//convertScaleAbs(grad_y, abs_grad_y);
		//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, thresholdFrame);
		//for (int i = 0; i < thresholdFrame.rows; i++) {
		//	for (int j = 0; j < thresholdFrame.cols; j++) {
		//		//thresholdFrame.at<uchar>(i, j) != 0 ? thresholdFrame.at<uchar>(i, j) = 255 : thresholdFrame.at<uchar>(i, j) = 0;
		//		thresholdFrame.at<uchar>(i, j) = thresholdFrame.at<uchar>(i, j) > 50  ? 255 : 0;
		//	}

		//}
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//threshold(blurredFrame, thresholdFrame, 100, 255, THRESH_BINARY_INV);
		// GRAFO

		//ximgproc::segmentation::GraphSegmentation::processImage(blurredFrame, thresholdFrame);
		//ximgproc::segmentation::GraphSegmentation::processImage()
		
		// GRAFo

		//adaptiveThreshold(blurredFrame, thresholdFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);
		//adaptiveThreshold(blurredFrame, thresholdFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);
		//adaptiveThreshold(thresholdFrame, thresholdFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);
		//int kernel_size = 3;
		//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(3 * 3);
		//Mat morph;
		//imwrite("close.png", thresholdFrame);
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_OPEN, kernel);
		//kernel = Mat::ones(11, 11, CV_32F) / (float)(11 * 11);
		
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_CLOSE, kernel);
		//kernel = Mat::ones(3, 3, CV_32F) / (float)(3 * 3);
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_OPEN, kernel);
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_CLOSE, kernel);
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_CLOSE, kernel);
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_OPEN, Mat::ones(7, 7, CV_32F) / (float)(7 * 7));
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));

		/*vector<vector<Point> > contours;
		findContours(thresholdFrame, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		Mat markers = Mat::zeros(thresholdFrame.size(), CV_32S);

		drawContours(markers, contours, -1, 255, -1);
		

		markers.copyTo(thresholdFrame);*/

		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//dilate(thresholdFrame, thresholdFrame, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		//Mat im_floodfill = thresholdFrame.clone();
		//floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));

		//// Invert floodfilled image
		//Mat im_floodfill_inv;
		//bitwise_not(im_floodfill, im_floodfill_inv);

		//// Combine the two images to get the foreground.
		//Mat im_out = (thresholdFrame | im_floodfill_inv);
		//thresholdFrame = im_out.clone();
		//morphologyEx(thresholdFrame, thresholdFrame, MORPH_OPEN, Mat::ones(3, 3, CV_32F) / (float)(3 * 3));
		
		//dilate(thresholdFrame, thresholdFrame, kernel);
		
		//imwrite("opening_20segundos.png", thresholdFrame);
		//Mat im_floodfill = thresholdFrame.clone();
		//floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));

		//// Invert floodfilled image
		//Mat im_floodfill_inv;
		//bitwise_not(im_floodfill, im_floodfill_inv);

		//// Combine the two images to get the foreground.
		//Mat im_out = (thresholdFrame | im_floodfill_inv);
		//im_out.copyTo(thresholdFrame);
		//medianBlur(thresholdFrame, thresholdFrame, 5);


		//imwrite("img_gauss.png", im_floodfill_inv);


		//return thresholdFrame;

		//return thresholdFrame;
		// ==================================================
		//Mat kernel = (Mat_<float>(3, 3) <<
		//	1, 1, 1,
		//	1, -8, 1,
		//	1, 1, 1); // an approximation of second derivative, a quite strong kernel
		//	// do the laplacian filtering as it is
		//	// well, we need to convert everything in something more deeper then CV_8U
		//	// because the kernel has some negative values,
		//	// and we can expect in general to have a Laplacian image with negative values
		//	// BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
		//	// so the possible negative number will be truncated
		//Mat imgLaplacian;
		//filter2D(cropped, imgLaplacian, CV_32F, kernel);
		//Mat sharp;
		//cropped.convertTo(sharp, CV_32F);
		//Mat imgResult = sharp - imgLaplacian;
		//// convert back to 8bits gray scale
		//imgResult.convertTo(imgResult, CV_8UC3);
		//imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
		//// imshow( "Laplace Filtered Image", imgLaplacian );
		////imshow("New Sharped Image", imgResult);
		//// Otsu
		////adaptiveThreshold(imgResult, otsu, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
		////threshold(imgResult, imgResult, 40, 255, THRESH_BINARY | THRESH_OTSU);
		//imwrite("New Sharped Image.png", imgResult);
		
	}
};

