#pragma once
#include <iostream>
#include <time.h>
#include <fstream>
#include <cmath>
#include <locale>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>
#include "PreProcessing.h"
#include "Segmentation.h"

using namespace std;
using namespace cv;


class ReadVideo {
private:
	Mat inputFrame;
	Mat outputFrame;
	string videoName;

	// Estrutura de um elemento para rastreamento, incluindo o Tracking, bounding box
	// status do rastreamento e id do elemento
	struct TrackerElement {
		// Tracker do elemento
		Ptr<Tracker> tracker;
		// Bbox atual
		Rect2d bbox;
		// Bbox anterior
		Rect2d prev_bbox[4];
		// Status do rastreamento
		bool status;
		// ID do elemento
		int id;
		// Contador de quantos frames que ele esta sendo rastreado
		int frame_counter = 0;
		// Frame em que o objeto entrou
		Mat entry_frame;
		// Frame em que o objeto foi detectado
		int first_frame;
		// Quantidade de frames desde que o objeto perdeu o tracking
		int exit_counter = 0;
		// Frame que o objeto saiu
		int last_frame;
		bool first_exit = true;
	};

public:
	ReadVideo(string videoName);

	// Le e processa o video
	bool read_video(string video_name, int min_time, int min_width, int min_height, int frames_to_skip);

};

