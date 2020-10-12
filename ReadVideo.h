#pragma once
#include <iostream>
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

	struct TrackerElement {
		Ptr<Tracker> tracker;
		Rect2d bbox;
		bool status;
	};

public:
	ReadVideo(string videoName) {
		cout << "VideoName: " << videoName<<endl;
		VideoCapture inputVideo(videoName); // video de entrada
		Mat frame; // frames do video
		if (!inputVideo.isOpened()) {
			cout << "Falha ao abrir o video"<<endl;
			return;
		} else {
			cout << "Video aberto com sucesso" << endl;
		}
		int video_width = inputVideo.get(CAP_PROP_FRAME_WIDTH);
		int video_height = inputVideo.get(CAP_PROP_FRAME_HEIGHT);
		cout << "FPS: " << inputVideo.get(CAP_PROP_FPS) << endl;
		
		VideoWriter output_video("teste_canny.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), inputVideo.get(CAP_PROP_FPS), Size(1180, 630), false);
		//VideoWriter output_teste("out_teste.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), inputVideo.get(CAP_PROP_FPS), Size(1180, 630), false);
		if (!output_video.isOpened()) {
			cout << "Falha ao abrir o video de saida" << endl;
			return;
		} else {
			cout << "Video de saida aberto com sucesso" << endl;
		}
		Mat preprocessed_frame;
		vector<Rect>rectangles;
		PreProcessing pre_processing;
		Segmentation segmentation;

		
		//Ptr<Tracker> tracker = TrackerKCF::create();
		// Cria o multi tracker
		//Ptr<MultiTracker> multi_tracker = cv::MultiTracker::create();
		MultiTracker multi_tracker;
		

		int count = 0;
		int frame_counter = 0;
		bool tracker_update_status = false;
		//cout << "Primeiro status: " << tracker_update_status;
		inputVideo.read(frame);
		Mat im_out;
		//im_out = pre_processing.pre_process(frame);
		//imwrite("Preprocessed.png", im_out);
		//Mat frame_pre_processed;
		preprocessed_frame = pre_processing.pre_process(frame);
		rectangles = segmentation.selective_search(preprocessed_frame);
		//vector<Ptr<Tracker>> algorithms;
		vector<Rect2d> objects;
		//vector<Ptr<Tracker>> tracker_vector;
		vector<TrackerElement> tracker_vector;
		TrackerElement tracker_element;
		// Inicia as structs dos trackers
		for (int i = 0; i < rectangles.size(); i++) {
			tracker_element.tracker = TrackerKCF::create();
			tracker_element.bbox = Rect2d(rectangles[i]);
			tracker_vector.push_back(tracker_element);
			//tracker_vector.push_back(TrackerKCF::create());
			//algorithms.push_back(TrackerKCF::create());
			//objects.push_back(rectangles[i]);
		}
		// Inicia os rastreadores
		for (int i = 0; i < tracker_vector.size(); i++) {
			tracker_vector[i].tracker->init(preprocessed_frame, rectangles[i]);
		}
		//objects.push_back(rectangles[0]);
		//objects.push_back(rectangles[1]);
		//multi_tracker.add(algorithms, preprocessed_frame, objects);

		/*for (int i = 0; i < rectangles.size(); i++) {
			multi_tracker->add(TrackerKCF::create(), preprocessed_frame, rectangles[i]);
		}*/

		//for (unsigned i = 0; i < multi_tracker.getObjects().size(); i++) {
		//	//tracker_update_status = multi_tracker->getObjects()[i];
		//	//cout << "Status: " << tracker_update_status << endl;
		//	im_out = preprocessed_frame.clone();
		//	rectangle(im_out, multi_tracker.getObjects()[i], Scalar(0, 255, 0), 2, 1);
		//}
		//imwrite("imagem3.png", im_out);
		Rect2d new_bbox;
		vector<Rect2d> returned_rects;
		//Mat im_out;
		do { // Processa o video, loop principal
			inputVideo.read(frame);
			
			preprocessed_frame = pre_processing.pre_process(frame);
			rectangles = segmentation.selective_search(preprocessed_frame);
			//if (frame_counter == 0) {
			//	for (int i = 0; i < rectangles.size(); i++) {
			//		multi_tracker->add(TrackerKCF::create(), frame, Rect2d(rectangles[i]));
			//	}
			//}

			//tracker_update_status = multi_tracker.update(preprocessed_frame, returned_rects);
			for (int i = 0; i < tracker_vector.size(); i++) {
				tracker_update_status = tracker_vector[i].tracker->update(preprocessed_frame, new_bbox);
				tracker_vector[i].status = tracker_update_status;
				if (tracker_update_status) {
					tracker_vector[i].bbox = new_bbox;
				}
				
				cout << "Tracker "<< tracker_vector[i].bbox.area()<<" | Status: " << tracker_vector[i].status << endl;
			}
			//cout << "Status: " << tracker_update_status << endl;

			//for (unsigned i = 0; i < multi_tracker.getObjects().size(); i++) {
			//for (unsigned i = 0; i < returned_rects.size(); i++) {
			//	//tracker_update_status = multi_tracker->getObjects()[i];
			//	//cout << "Status: " << tracker_update_status << endl;
			//	im_out = preprocessed_frame.clone();
			//	//rectangle(im_out, multi_tracker.getObjects()[i], Scalar(0, 255, 0), 2, 1);
			//	rectangle(im_out, returned_rects[i], Scalar(0, 255, 0), 2, 1);
			//}
			//imwrite("imagem2.png", im_out);
			//cvtColor(frame, preprocessed_frame, COLOR_BGR2GRAY);
			// Writes frame to video
			//output_video.write(preprocessed_frame);
			//output_teste.write(frame);
			//imshow("MultiTracker", frame);
			//count=600;
			frame_counter++;
			//cout << count << endl;

		}while(!frame.empty() && count != 600);

		inputVideo.release();
		output_video.release();
		cout << "Fim do loop" << endl;
		



	}
};

