#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>
#include "PreProcessing.h"
using namespace std;
using namespace cv;

class ReadVideo {
private:
	Mat inputFrame;
	Mat outputFrame;
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
		PreProcessing variavel;
		int count = 0;
		do { // Processa o video, loop principal
			inputVideo.read(frame);
			//Range rows(1, 100);
			//Range cols(1, 100);
			// Retangulo para cortar a imagem (tirar o logo e o ROV)
			/*Mat crop_rect(frame, Rect(35, 90, 1180, 630));
			Mat cropped;
			crop_rect.copyTo(cropped);
			imwrite("cropped.png", cropped);*/
			//PreProcessing variavel(frame);
			//preprocessed_frame = variavel.pre_process(frame);
			preprocessed_frame = variavel.selective_search(frame);
			//cvtColor(frame, preprocessed_frame, COLOR_BGR2GRAY);
			// Writes frame to video
			output_video.write(preprocessed_frame);
			//output_teste.write(frame);
			count++;
			//cout << count << endl;

		}while(!frame.empty() && count != 600);
		inputVideo.release();
		output_video.release();
		cout << "Fim do loop" << endl;
		



	}
};

