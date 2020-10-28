#include "ReadVideo.h"

//#include <iostream>
//#include <time.h>
//#include <fstream>
//#include <opencv2/core.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/tracking.hpp>
//#include "PreProcessing.h"
//#include "Segmentation.h"

using namespace std;
using namespace cv;

ReadVideo::ReadVideo(string videoName) {

}

bool ReadVideo::read_video(string videoName) {

	cout << "VideoName: " << videoName<<endl;
	VideoCapture inputVideo(videoName); // video de entrada
	Mat frame; // frames do video
	if (!inputVideo.isOpened()) {
		cout << "Falha ao abrir o video"<<endl;
		return false;
	} else {
		cout << "Video aberto com sucesso" << endl;
	}
	int video_width = inputVideo.get(CAP_PROP_FRAME_WIDTH);
	int video_height = inputVideo.get(CAP_PROP_FRAME_HEIGHT);
	cout << "FPS: " << inputVideo.get(CAP_PROP_FPS) << endl;
	int fps = inputVideo.get(CAP_PROP_FPS);
	//VideoWriter output_video("teste_canny.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), inputVideo.get(CAP_PROP_FPS), Size(1180, 630), false);
	//VideoWriter output_teste("out_teste.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), inputVideo.get(CAP_PROP_FPS), Size(1180, 630), false);
	//if (!output_video.isOpened()) {
	//	cout << "Falha ao abrir o video de saida" << endl;
	//	return false;
	//} else {
	//	cout << "Video de saida aberto com sucesso" << endl;
	//}
	
	// Frame pre-processado
	Mat preprocessed_frame;
	// Rects retornados pela segmentacao
	vector<Rect2d> rectangles;
	// Variavel da classe PreProcessamento
	PreProcessing pre_processing;
	// Variavel da classe Segmentacao
	Segmentation segmentation;
	
	// Contador de quantos frames passaram no video
	int frame_counter = 0;
	// Contador das IDs dos objetos
	int id_counter = 0;
	int fix_id_counter = 0;
	// Status da atualizacao do tracker
	bool tracker_update_status = false;
	
	// Arquivo de saida, com os dados dos objetos
	// Deve ser escrito ID_OBJETO,FRAME_ENTRADA,FRAME_SAIDA,TEMPO_QUE_FICOU
	ofstream csv_file;
	csv_file.open("Log_de_Saida.csv");
	ofstream out_log;
	out_log.open("Relatorio_Programa.txt");

	csv_file << "ID;Frame Entrada;Frame Saida;Tempo no Video;Tamanho\n";

	inputVideo.read(frame);
	
	//im_out = pre_processing.pre_process(frame);
	//imwrite("Preprocessed.png", im_out);
	//Mat frame_pre_processed;
	// Realiza o pre-processamento do primeiro frame
	preprocessed_frame = pre_processing.pre_process(frame);

	rectangles = segmentation.selective_search(preprocessed_frame);
	vector<Rect2d> previous_trackers;
	vector<TrackerElement> tracker_vector;
	TrackerElement tracker_element;
	// Inicia as structs dos trackers
	for (int i = 0; i < rectangles.size(); i++) {
		tracker_element.tracker = TrackerKCF::create();
		tracker_element.tracker->init(preprocessed_frame, rectangles[i]);
		tracker_element.bbox = rectangles[i];
		tracker_element.prev_bbox[0] = tracker_element.bbox;
		tracker_element.prev_bbox[1] = tracker_element.bbox;
		tracker_element.prev_bbox[2] = tracker_element.bbox;
		tracker_element.prev_bbox[3] = tracker_element.bbox;
		//tracker_element.prev_bbox = rectangles[i];
		tracker_element.id = id_counter;
		tracker_element.entry_frame = Mat(preprocessed_frame, rectangles[i]);
		tracker_element.first_frame = frame_counter;
		tracker_vector.push_back(tracker_element);
		//cout << "Elemento detectado no frame " << frame_counter << ". ID = " << tracker_element.id << endl;
		id_counter++;
		//tracker_vector.push_back(TrackerKCF::create());
		//algorithms.push_back(TrackerKCF::create());
		//objects.push_back(rectangles[i]);
	}
	// Inicia os rastreadores
	/*for (int i = 0; i < tracker_vector.size(); i++) {
		tracker_vector[i].tracker->init(preprocessed_frame, rectangles[i]);
	}*/

	Rect2d new_bbox;
	// Vetor com as bbox dos frames antigos
	vector<Rect2d> prev_bbox[4];
	// Frames para pular antes de fazer a segmentacao, para diminuir o tempo
	int frames_to_skip = 4;
	// Iterar por todos os rects da segmentacao
	// caso tenha interpolacao com os rects atualizados o tracker, descartar,
	// se nao tiver interpolacao, cria um tracker novo
	Mat im_out;
	string img_output_string;
	
	int objects_counter = 0;


	clock_t start, end;
	double time_taken;
	
	frame_counter++;
	inputVideo.read(frame);
	do { // Processa o video, loop principal
		start = clock();
		if (frame_counter == 697) {
			cout << "Deu ruim" << endl;
		}
		//inputVideo.read(frame);
		cout << "Loop " << frame_counter << ".Tempo:" << floor(frame_counter/fps) << endl;
		preprocessed_frame = pre_processing.pre_process(frame);
		// Realiza o pre-processamento e a segmentacao de n em n frames,
		// definidos pela variavel frames to skip
		if (frame_counter % frames_to_skip == 0) {
			rectangles = segmentation.selective_search(preprocessed_frame);
			//cout << "Fez SSSSSSSSSSSSSSSSSSSSSSSSS" << endl;
		}

		//tracker_update_status = multi_tracker.update(preprocessed_frame, returned_rects);
		cout << "Tamanho rect = " << rectangles.size() << endl;
		for (int i = 0; i < tracker_vector.size(); i++) {
			tracker_update_status = tracker_vector[i].tracker->update(preprocessed_frame, new_bbox);
			tracker_vector[i].status = tracker_update_status;
			tracker_vector[i].frame_counter++;
			// Se o tracking foi um sucesso, atualiza o bbox atual e antigos
			if (tracker_vector[i].status) {
				tracker_vector[i].prev_bbox[0] = tracker_vector[i].bbox;
				tracker_vector[i].prev_bbox[1] = tracker_vector[i].prev_bbox[0];
				tracker_vector[i].prev_bbox[2] = tracker_vector[i].prev_bbox[1];
				tracker_vector[i].prev_bbox[3] = tracker_vector[i].prev_bbox[2];
				tracker_vector[i].bbox = new_bbox;
				if (tracker_vector[i].frame_counter == inputVideo.get(CAP_PROP_FPS)) {
					cout << "Elemento detectado no frame " << tracker_vector[i].first_frame << ". ID = " << tracker_vector[i].id << endl;
					out_log << "Elemento detectado no tempo " + to_string(frame_counter / fps) + " segundos. ID = " + to_string(tracker_vector[i].id) + "\n";
					img_output_string = "Imagens_Objetos\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Entrada.png";
					objects_counter++;
					imwrite(img_output_string, tracker_vector[i].entry_frame);
				}
			// Se o tracker nao seguir por 30 frames, tira do vector
			} else if (tracker_vector[i].frame_counter < inputVideo.get(CAP_PROP_FPS) + inputVideo.get(CAP_PROP_FPS)/2) {
				tracker_vector.erase(tracker_vector.begin() + i);
				//cout << "Apagou ID = " << tracker_vector[i].id << endl;
				//if (tracker_vector[i].frame_counter >= 3) {
					//csv_file << tracker_vector[i].id << ";" << tracker_vector[i].entry_frame << ";" << frame_counter << "\n";
					//cout << "Apagou ID = " << tracker_vector[i].id << endl;
				//}
				//tracker_vector.erase(tracker_vector.begin() + i);

			} else {
				cout << "Apagou ID = " << tracker_vector[i].id << endl;
				out_log << "Apagou ID = " + to_string(tracker_vector[i].id) + " no tempo " + to_string(frame_counter / fps) + " segundos\n";
				csv_file << tracker_vector[i].id << ";" << tracker_vector[i].first_frame / fps << ";" << frame_counter / fps << 
					";" << (int)(frame_counter / fps - tracker_vector[i].first_frame / fps) << ";" << tracker_vector[i].bbox.area() << "\n";
				img_output_string = "Imagens_Objetos\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Saida.png";
				//imwrite(img_output_string, Mat(preprocessed_frame, tracker_vector[i].bbox));
				tracker_vector.erase(tracker_vector.begin() + i);
			}

			if (frame_counter % frames_to_skip == 0) {
				for (int j = 0; j < rectangles.size(); j++) {
					// Tem interpolacao entre o bbox detectado e um bbox sendo rastreado,
					// logo a bbox detectada deve ser ignorada
					if ((rectangles[j] & tracker_vector[i].bbox).area() > 0) {
						//cout << "Teve interpolacao | " << rectangles[j].area() << " e " << tracker_vector[i].bbox.area() << endl;
						rectangles.erase(rectangles.begin() + j);
						j--;
					}

					for (int k = 0; k < 4; k++) {
						// Tem interpolacao entre o bbox atual e algum outro que estava sendo rastreado,
						// logo, inicializa o tracker de novo
						if (tracker_vector[i].status == 0 && (rectangles[j] & tracker_vector[i].prev_bbox[k]).area() > 0) {
							tracker_vector[i].tracker = TrackerKCF::create();
							tracker_vector[i].tracker->init(preprocessed_frame, rectangles[j]);
							tracker_vector[i].bbox = rectangles[j];
							tracker_vector[i].prev_bbox[0] = tracker_vector[i].bbox;
							tracker_vector[i].prev_bbox[1] = tracker_vector[i].prev_bbox[0];
							tracker_vector[i].prev_bbox[2] = tracker_vector[i].prev_bbox[1];
							tracker_vector[i].prev_bbox[3] = tracker_vector[i].prev_bbox[2];
							cout << "Recuperou ID " << tracker_vector[i].id << endl;
							rectangles.erase(rectangles.begin() + j);
							j--;
							break;
						}
					}

				}
			}

			if (tracker_vector[i].frame_counter > inputVideo.get(CAP_PROP_FPS) + inputVideo.get(CAP_PROP_FPS) / 2) {
				cout << "Tracker ID: " << tracker_vector[i].id << " | Area: " << tracker_vector[i].bbox.area() << "\t\t| Status: " << tracker_vector[i].status << " cont: "<< tracker_vector[i].frame_counter << endl;

			}

		
		}
		if (frame_counter % frames_to_skip == 0) {
			// Cria trackers para os novos elementos detectados
			for (int i = 0; i < rectangles.size(); i++) {
				tracker_element.tracker = TrackerKCF::create();
				tracker_element.tracker->init(preprocessed_frame, rectangles[i]);
				tracker_element.bbox = rectangles[i];
				//tracker_element.prev_bbox = rectangles[i];
				tracker_element.id = id_counter;
				tracker_element.entry_frame = Mat(preprocessed_frame, rectangles[i]);
				tracker_element.first_frame = frame_counter;
				tracker_vector.push_back(tracker_element);
				//cout << "Elemento detectado no frame " << frame_counter << " || ID = " << tracker_element.id << endl;
				id_counter++;
				
			}
		}

		//imwrite("imagem2.png", im_out);
		// Writes frame to video
		//output_video.write(preprocessed_frame);
		//output_teste.write(frame);
		//count=600;
		frame_counter++;
		/*for (int i = 0; i < tracker_vector.size(); i++) {
			if (tracker_vector[i].status && tracker_vector[i].frame_counter > 1) {
				rectangle(preprocessed_frame, tracker_vector[i].bbox, Scalar(255, 0, 0), 2, 1);
			}
		}
		imshow("Frame", preprocessed_frame);
		waitKey(0);*/
		cout << "\n\n\n";
		//waitKey();
		//cout << count << endl;
		end = clock();
		time_taken = double(end - start) / double(CLOCKS_PER_SEC);
		cout << "Tempo do loop: "  << time_taken << setprecision(5) << endl;
		//waitKey(0);
		//getchar();
		inputVideo.read(frame);
	}while(!frame.empty());

	for (int i = 0; i < tracker_vector.size(); i++) {
		cout << "Apagou ID = " << tracker_vector[i].id << endl;
		out_log << "Apagou ID = " + to_string(tracker_vector[i].id) + " no tempo "  + to_string((int)floor(frame_counter / fps)) + " segundos\n";
		//csv_file << tracker_vector[i].id << ";" << tracker_vector[i].first_frame << ";" << frame_counter << tracker_vector[i].bbox.area() << "\n";
		csv_file << tracker_vector[i].id << ";" << tracker_vector[i].first_frame / fps << ";" << frame_counter / fps << ";" << 
			frame_counter / fps - tracker_vector[i].first_frame / fps << ";" << tracker_vector[i].bbox.area() << "\n";
	}

	cout << "Quantidade de objetos detectados: " << objects_counter << endl;

	csv_file.close();
	inputVideo.release();
	//output_video.release();

	cout << "Fim do loop" << endl;
	return true;
}


//for (int j = 0; j < 4; j++) {
//	for (int k = 0; k < prev_bbox[j].size(); k++) {
//		// Tem interpolacao entre o bbox atual e algum outro que estava sendo rastreado,
//		// logo, inicializa o tracker de novo
//		if ((rectangles[i] & prev_bbox[j][k]).area() > 0) {
//			tracker_vector[i].tracker->init(preprocessed_frame, prev_bbox[j][k]);
//		}
//	}
//}