#include "Segmentation.h"

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

Segmentation::Segmentation() {

}

vector<Rect2d> Segmentation::selective_search(Mat frame, int k) {

	clock_t start, end;
	start = clock();
	//Mat crop_rect(frame, Rect(35, 90, 1180, 630));
	//crop_rect.copyTo(frame);
	//medianBlur(frame, frame, 7);
	// speed-up using multithreads
	//setUseOptimized(true);
	//setNumThreads(4);

	// resize image
	//int newHeight = 200;
	//int newWidth = frame.cols * newHeight / frame.rows;
	//resize(frame, frame, Size(newWidth, newHeight));



	// Creates selective search
	Ptr<SelectiveSearchSegmentation> selective_search = createSelectiveSearchSegmentation();
	// Set input image
	selective_search->setBaseImage(frame);
	// Select single strategy, with k = 850, its faster, and produces less, but more important, results
	selective_search->switchToSingleStrategy(k);

	// creates a Rect vector to store the ractangles that selective search finds
	vector<Rect> rects;
	selective_search->process(rects);
	std::cout << "Numero de regioes: " << rects.size() << std::endl;
	// Sorts the vector, to have the rects with the biggest area first
	sort(rects.begin(), rects.end(), [](const Rect& left, const Rect& right) {
		return left.area() > right.area();
		}
	);

	Mat imOut = frame.clone();
	for (int i = 0; i < rects.size(); i++) {
		rectangle(imOut, rects[i], Scalar(0, 255, 0));
	}
	imwrite("Segmentacao_Original.png", imOut);

	// Removes the rect that encompasses the whole image
	rects.erase(rects.begin());

	for (int i = 0; i < rects.size(); i++) {
		for (int j = i + 1; j < rects.size(); j++) {
			//cout << "i = " << i << ", j = " << j << ", resultado: " << ((rects[i] & rects[j]).area() > 0) << endl;
			// If the rects intersect
			if ((rects[i] & rects[j]).area() > 0) {
				rects[i] = (rects[i] | rects[j]);
				rects.erase(rects.begin() + j);
				j--;
			}
		}
	}

	imOut = frame.clone();
	for (int i = 0; i < rects.size(); i++) {
		rectangle(imOut, rects[i], Scalar(0, 255, 0));
	}
	imwrite("Segmentacao_Sem_Interpolacao.png", imOut);


	vector < Rect2d> rects_2d;

	for (int i = 0; i < rects.size(); i++) {
		rects_2d.push_back(Rect2d(rects[i].x, rects[i].y, rects[i].width, rects[i].height));
		//rects_2d.push_back(Rect2d(rects[i]));
	}


	//Mat imOut = frame.clone();

	//for (int i = 0; i < rects.size(); i++) {
	//	rectangle(imOut, rects[i], Scalar(0, 255, 0));
	//}
	//imwrite("imagem.png", imOut);
	end = clock();
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	//cout << "Tempo da segmentacao: " << fixed << time_taken << setprecision(5) << endl;
	return rects_2d;
}