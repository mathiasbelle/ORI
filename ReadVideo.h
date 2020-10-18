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
		// Tracker do elemento
		Ptr<Tracker> tracker;
		// Bbox atual
		Rect2d bbox;
		// Bbox anterior
		Rect2d prev_bbox;
		// Status do rastreamento
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
		vector<Rect2d>rectangles;
		PreProcessing pre_processing;
		Segmentation segmentation;

		
		//Ptr<Tracker> tracker = TrackerKCF::create();
		// Cria o multi tracker
		//Ptr<MultiTracker> multi_tracker = cv::MultiTracker::create();
		//MultiTracker multi_tracker;
		

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
		vector<Rect2d> previous_trackers;
		//vector<Ptr<Tracker>> tracker_vector;
		vector<TrackerElement> tracker_vector;
		TrackerElement tracker_element;
	
		bool interpolation = false;
		// Inicia as structs dos trackers
		for (int i = 0; i < rectangles.size(); i++) {
			tracker_element.tracker = TrackerKCF::create();
			tracker_element.bbox = rectangles[i];
			tracker_element.prev_bbox = rectangles[i];
			tracker_vector.push_back(tracker_element);
			//tracker_vector.push_back(TrackerKCF::create());
			//algorithms.push_back(TrackerKCF::create());
			//objects.push_back(rectangles[i]);
		}
		// Inicia os rastreadores
		for (int i = 0; i < tracker_vector.size(); i++) {
			tracker_vector[i].tracker->init(preprocessed_frame, rectangles[i]);
		}

		Rect2d new_bbox;
		vector<Rect2d> returned_rects;
		// Iterar por todos os rects da segmentacao
		// caso tenha interpolacao com os rects atualizados o tracker, descartar,
		// se nao tiver interpolacao, cria um tracker novo
		//Mat im_out;
		do { // Processa o video, loop principal
			inputVideo.read(frame);
			cout << "Loop " << frame_counter << endl;
			preprocessed_frame = pre_processing.pre_process(frame);
			rectangles = segmentation.selective_search(preprocessed_frame);

			//tracker_update_status = multi_tracker.update(preprocessed_frame, returned_rects);
			cout << "Tamanho rect = " << rectangles.size() << endl;
			for (int i = 0; i < tracker_vector.size(); i++) {
				tracker_update_status = tracker_vector[i].tracker->update(preprocessed_frame, new_bbox);
				tracker_vector[i].status = tracker_update_status;
				// Se o tracking foi um sucesso, atualiza o bbox
				if (tracker_update_status) {
					tracker_vector[i].prev_bbox = tracker_vector[i].bbox;
					tracker_vector[i].bbox = new_bbox;
				}
				for (int j = 0; j < rectangles.size(); j++) {
					// Tem interpolacao entre o bbox detectado e um bbox sendo rastreado,
					// logo a bbox detectada deve ser ignorada
					if ((rectangles[i] & tracker_vector[i].bbox).area() > 0) {
						cout << "Teve interpolacao | " << rectangles[i].area() << " e " << tracker_vector[i].bbox.area() << endl;
						rectangles.erase(rectangles.begin() + j);
						j--;
					// Teve interpolacao com um bbox do frame antigo, que se perdeu
					//} else if ((Rect2d(rectangles[i]) & tracker_vector[i].prev_bbox).area() > 0) {

					}
				}
				
				
				cout << "Tracker "<< tracker_vector[i].bbox.area()<<"\t\t| Status: " << tracker_vector[i].status << endl;
			}
			//cout << "Tamanho rect depois da verificacao = " << rectangles.size() << endl;
			// Cria um tracker para cada novo elemento detectado na segmentacao
			//for (int i = 0; i < rectangles.size(); i++) {
			//	//tracker_element.tracker = TrackerKCF::create();
			//	tracker_element.tracker = TrackerMedianFlow::create();
			//	tracker_element.bbox = Rect2d(rectangles[i]);
			//	// Inicia o tracker
			//	tracker_element.tracker->init(preprocessed_frame, rectangles[i]);
			//	tracker_vector.push_back(tracker_element);
			//	cout << "Novo elemento adicionado." <<	endl;
			//}

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
			//for (int i = 0; i < tracker_vector.size(); i++) {
			//	if (tracker_vector[i].status) {
			//		rectangle(preprocessed_frame, tracker_vector[i].bbox, Scalar(255, 0, 0), 2, 1);
			//	}
			//}
			//imshow("Frame", preprocessed_frame);
			//waitKey(0);
			cout << "\n\n\n";
			//waitKey();
			//cout << count << endl;

		}while(!frame.empty() && count != 600);

		inputVideo.release();
		output_video.release();
		cout << "Fim do loop" << endl;
		



	}
};

