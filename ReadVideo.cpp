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
using namespace std::filesystem;
using namespace cv;


ReadVideo::ReadVideo(string videoName) {

}

bool ReadVideo::read_video(string videoName, int min_time, int min_width, int min_height, int frames_to_skip) {
	clock_t start_func, end_func;
	start_func = clock();
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
	
	int fps = (int)ceil(inputVideo.get(CAP_PROP_FPS));
	cout << "FPS: " << fps << endl;
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
	path video_path(videoName);
	string dir_name = video_path.replace_extension("").string();
	cout << "Dirname: "<<dir_name << endl;
	create_directory(dir_name);

	// Arquivo de saida, com os dados dos objetos
	// Deve ser escrito ID_OBJETO,FRAME_ENTRADA,FRAME_SAIDA,FRAMES_QUE_FICOU,TEMPO_ENTRADA,TEMPO_SAIDA,TEMPO_QUE_FICOU,POSICAO_X,POSICAO_Y,LARGURA,ALTURA,AREA
	ofstream csv_file;
	// Deixa a formatacao de floats com a representacao da localidade do usuario do PC, do contrario a exibicao no excel
	// fica incorreta
	// No caso de portugues, virgula, ao inves de ponto
	csv_file.imbue(locale(""));
	csv_file.open(videoName + "_Log_de_Saida.csv");
	ofstream out_log;
	out_log.imbue(locale(""));
	out_log.open(videoName + "_Relatorio_Programa.txt");

	csv_file << "ID;Frame Entrada;Frame Saida;Frames que Ficou;Tempo Entrada;Tempo Saida;Tempo no Video;"
				"Posicao X;Posicao Y;Largura;Altura;Area\n";

	inputVideo.read(frame);
	//imwrite("efnjuefnjej.png", frame);
	
	//im_out = pre_processing.pre_process(frame);
	//imwrite("Preprocessed.png", im_out);
	//Mat frame_pre_processed;
	// Realiza o pre-processamento do primeiro frame
	preprocessed_frame = pre_processing.pre_process(frame);
	int ss_k = 850;
	rectangles = segmentation.selective_search(preprocessed_frame, ss_k);
	vector<Rect2d> previous_trackers;
	vector<TrackerElement> tracker_vector;
	TrackerElement tracker_element;
	
	// Inicia as structs dos trackers
	for (int i = 0; i < rectangles.size(); i++) {
		if (rectangles[i].width >= min_width && rectangles[i].height >= min_height) {
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
			tracker_element.exit_counter = 0;
			tracker_vector.push_back(tracker_element);
			//cout << "Elemento detectado no frame " << frame_counter << ". ID = " << tracker_element.id << endl;
			id_counter++;
			//tracker_vector.push_back(TrackerKCF::create());
			//algorithms.push_back(TrackerKCF::create());
			//objects.push_back(rectangles[i]);
		}
	}
	// Inicia os rastreadores
	/*for (int i = 0; i < tracker_vector.size(); i++) {
		tracker_vector[i].tracker->init(preprocessed_frame, rectangles[i]);
	}*/
	
	Rect2d new_bbox;
	// Vetor com as bbox dos frames antigos
	vector<Rect2d> prev_bbox[4];
	// Frames para pular antes de fazer a segmentacao, para diminuir o tempo
	//int frames_to_skip = 8;
	// Iterar por todos os rects da segmentacao
	// caso tenha interpolacao com os rects atualizados o tracker, descartar,
	// se nao tiver interpolacao, cria um tracker novo
	Mat im_out;
	string img_output_string;
	
	int objects_counter = 0;


	clock_t start_loop, end_loop;
	double time_taken;
	
	frame_counter++;
	//inputVideo.read(frame);
	Mat frame_copy;
	int lol;
	while (inputVideo.read(frame)) { // Processa o video, loop principal
		start_loop = clock();

		//inputVideo.read(frame);
		cout << "Loop " << frame_counter << ".Tempo:" << floor(frame_counter/fps) << endl;
		preprocessed_frame = pre_processing.pre_process(frame);
		preprocessed_frame.copyTo(frame_copy);
		// Realiza o pre-processamento e a segmentacao de n em n frames,
		// definidos pela variavel frames to skip
		if (frame_counter % frames_to_skip == 0) {
			cin >> lol;
			rectangles = segmentation.selective_search(preprocessed_frame, ss_k);
			
			//cout << "Fez SSSSSSSSSSSSSSSSSSSSSSSSS" << endl;
		}

		//if (frame_counter % frames_to_skip == 0) {
		//	for (int i = 0; i < rectangles.size(); i++) {
		//			rectangle(preprocessed_frame, rectangles[i], Scalar(255, 0, 0), 2, 1);
		//		
		//	}
		//	imshow("Frame", preprocessed_frame);
		//	waitKey(0);
		//}

		//tracker_update_status = multi_tracker.update(preprocessed_frame, returned_rects);
		cout << "Tamanho rect = " << rectangles.size() << endl;
		for (int i = 0; i < tracker_vector.size(); i++) {
			tracker_update_status = tracker_vector[i].tracker->update(preprocessed_frame, new_bbox);
			tracker_vector[i].status = tracker_update_status;
			tracker_vector[i].frame_counter++;
			// Se o tracking foi um sucesso, atualiza o bbox atual e antigos
			if (tracker_vector[i].status) {
				tracker_vector[i].bbox = new_bbox;
				tracker_vector[i].exit_counter = 0;
				tracker_vector[i].first_exit = true;
				//tracker_vector[i].prev_bbox[0] = tracker_vector[i].bbox;
				//tracker_vector[i].prev_bbox[1] = tracker_vector[i].prev_bbox[0];
				//tracker_vector[i].prev_bbox[2] = tracker_vector[i].prev_bbox[1];
				//tracker_vector[i].prev_bbox[3] = tracker_vector[i].prev_bbox[2];
				
				if (tracker_vector[i].frame_counter == min_time) {
					cout << "Elemento detectado no frame " << tracker_vector[i].first_frame << ". ID = " << tracker_vector[i].id << endl;
					out_log << "Elemento detectado no tempo " + to_string(frame_counter / fps) + " segundos. ID = " + to_string(tracker_vector[i].id) + "\n";
					//img_output_string = "Imagens_Objetos\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Entrada.png";
					img_output_string = dir_name + "\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Entrada.png";
					objects_counter++;
					imwrite(img_output_string, tracker_vector[i].entry_frame);
				}
			// Se o tracker nao seguir por 30 frames, tira do vector
			} else if (tracker_vector[i].frame_counter < min_time) {
				tracker_vector.erase(tracker_vector.begin() + i);
				//cout << "Apagou ID = " << tracker_vector[i].id << endl;
				//if (tracker_vector[i].frame_counter >= 3) {
					//csv_file << tracker_vector[i].id << ";" << tracker_vector[i].entry_frame << ";" << frame_counter << "\n";
					//cout << "Apagou ID = " << tracker_vector[i].id << endl;
				//}
				//tracker_vector.erase(tracker_vector.begin() + i);

			} else {
				// Grava o frame que o objeto saiu
				if (tracker_vector[i].exit_counter == 0) {
					tracker_vector[i].last_frame = frame_counter;
				}
				// Aumenta o contador de frames desde que o objeto saiu, pois o tracker pode recuperar o objeto
				tracker_vector[i].exit_counter++;
				// Se passou muito tempo, retira o objeto
				if (tracker_vector[i].exit_counter >= fps * 5) {
					cout << "Apagou ID = " << tracker_vector[i].id << endl;
					out_log << "Apagou ID = " + to_string(tracker_vector[i].id) + " no tempo " + to_string(tracker_vector[i].last_frame / fps) + " segundos\n";
					// Deve ser escrito ID_OBJETO,FRAME_ENTRADA,FRAME_SAIDA,FRAMES_QUE_FICOU,TEMPO_ENTRADA,TEMPO_SAIDA,TEMPO_QUE_FICOU,POSICAO_X,POSICAO_Y,LARGURA,ALTURA,AREA
					csv_file << tracker_vector[i].id << ";" <<				// ID_OBJETO
						tracker_vector[i].first_frame << ";" <<		// FRAME_ENTRADA
						tracker_vector[i].last_frame << ";" <<						// FRAME_SAIDA
						tracker_vector[i].last_frame - tracker_vector[i].first_frame << ";" << // FRAMES_QUE_FICOU
						static_cast<float>(tracker_vector[i].first_frame) / fps << ";" <<			// TEMPO_ENTRADA
						static_cast<float>(tracker_vector[i].last_frame) / fps << ";" <<							// TEMPO_SAIDA
						static_cast<float>(tracker_vector[i].last_frame) / fps - static_cast<float>(tracker_vector[i].first_frame) / fps << ";" << // TEMPO_QUE_FICOU
						tracker_vector[i].bbox.x << ";" <<		// POSICAO_X
						tracker_vector[i].bbox.y << ";" <<		// POSICAO_Y
						tracker_vector[i].bbox.width << ";" <<	// LARGURA
						tracker_vector[i].bbox.height << ";" << // ALTURA
						tracker_vector[i].bbox.area() << "\n";  // AREA
					//img_output_string = "Imagens_Objetos\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Saida.png";
					//imwrite(img_output_string, Mat(preprocessed_frame, tracker_vector[i].prev_bbox[0]));
					tracker_vector.erase(tracker_vector.begin() + i);
				}
			}

			if (frame_counter % frames_to_skip == 0) {
				for (int j = 0; j < rectangles.size(); j++) {
					// Tem interpolacao entre o bbox detectado e um bbox sendo rastreado,
					// logo a bbox detectada deve ser ignorada
					if ((rectangles[j] & tracker_vector[i].bbox).area() > 0) {
						//cout << "Teve interpolacao | " << rectangles[j].area() << " e " << tracker_vector[i].bbox.area() << endl;
						tracker_vector[i].prev_bbox[1] = tracker_vector[i].prev_bbox[0];
						tracker_vector[i].prev_bbox[2] = tracker_vector[i].prev_bbox[1];
						tracker_vector[i].prev_bbox[3] = tracker_vector[i].prev_bbox[2];
						tracker_vector[i].prev_bbox[0] = rectangles[j];
						rectangles.erase(rectangles.begin() + j);
						j--;
					}

					for (int k = 0; k < 4; k++) {
						 //Tem interpolacao entre o bbox atual e algum outro que estava sendo rastreado,
						 //logo, inicializa o tracker de novo
						if (tracker_vector[i].status == 0 && (rectangles[j] & tracker_vector[i].prev_bbox[k]).area() > 0) {
							//tracker_vector[i].tracker = TrackerKCF::create();
							tracker_vector[i].tracker->init(preprocessed_frame, rectangles[j]);
							tracker_vector[i].bbox = rectangles[j];
							tracker_vector[i].prev_bbox[0] = tracker_vector[i].bbox;
							tracker_vector[i].prev_bbox[1] = tracker_vector[i].bbox;
							tracker_vector[i].prev_bbox[2] = tracker_vector[i].bbox;
							tracker_vector[i].prev_bbox[3] = tracker_vector[i].bbox;
							tracker_vector[i].status = 1;
							tracker_vector[i].exit_counter = 0;
							cout << "Recuperou ID " << tracker_vector[i].id << endl;
							out_log << "Recuperou ID " + to_string(tracker_vector[i].id) + "\n";
							rectangles.erase(rectangles.begin() + j);
							j--;
							break;
						}
					}
				}
			}

			if (tracker_vector[i].frame_counter > min_time && frame_counter % frames_to_skip == 0) {
				cout << "Tracker ID: " << tracker_vector[i].id << " | Area: " << tracker_vector[i].bbox.area() << "\t\t| Status: " << tracker_vector[i].status << " cont: "<< tracker_vector[i].frame_counter << endl;

			}

		
		}
		if (frame_counter % frames_to_skip == 0) {
			// Cria trackers para os novos elementos detectados
			for (int i = 0; i < rectangles.size(); i++) {
				if (rectangles[i].width >= min_width && rectangles[i].height >= min_height) {
					tracker_element.tracker = TrackerKCF::create();
					tracker_element.tracker->init(preprocessed_frame, rectangles[i]);
					tracker_element.bbox = rectangles[i];
					//tracker_element.prev_bbox = rectangles[i];
					tracker_element.id = id_counter;
					tracker_element.entry_frame = Mat(preprocessed_frame, rectangles[i]);
					tracker_element.first_frame = frame_counter;
					tracker_element.exit_counter = 0;
					tracker_vector.push_back(tracker_element);
					//cout << "Elemento detectado no frame " << frame_counter << " || ID = " << tracker_element.id << endl;
					id_counter++;
				}
			}
		}

		//imwrite("imagem2.png", im_out);
		// Writes frame to video
		//output_video.write(preprocessed_frame);
		//output_teste.write(frame);
		//count=600;
		//if (frame_counter % frames_to_skip == 0) {
		//	for (int i = 0; i < tracker_vector.size(); i++) {
		//		if (tracker_vector[i].status && tracker_vector[i].frame_counter > 1) {
		//			rectangle(preprocessed_frame, tracker_vector[i].bbox, Scalar(255, 0, 0), 2, 1);
		//		}
		//	}
		//	imshow("Frame", preprocessed_frame);
		//	waitKey(0);
		//}

		frame_counter++;

		cout << "\n\n\n";
		//waitKey();
		//cout << count << endl;
		end_loop = clock();
		time_taken = double(end_loop - start_loop) / double(CLOCKS_PER_SEC);
		cout << "Tempo do loop: "  << time_taken << setprecision(5) << endl;
		//waitKey(0);
		//getchar();
		//inputVideo.read(frame);
	}//while(!frame.empty());

	for (int i = 0; i < tracker_vector.size(); i++) {
		if (tracker_vector[i].frame_counter >= fps) {
			if (tracker_vector[i].exit_counter == 0) {
				tracker_vector[i].last_frame = frame_counter;
			}
			cout << "Apagou ID = " << tracker_vector[i].id << endl;
			out_log << "Apagou ID = " + to_string(tracker_vector[i].id) + " no tempo " + to_string(static_cast<int>(floor(tracker_vector[i].last_frame / fps))) + " segundos\n";
			//csv_file << tracker_vector[i].id << ";" << tracker_vector[i].first_frame << ";" << frame_counter << tracker_vector[i].bbox.area() << "\n";
			csv_file << tracker_vector[i].id << ";" <<				// ID_OBJETO
				tracker_vector[i].first_frame << ";" <<		// FRAME_ENTRADA
				tracker_vector[i].last_frame << ";" <<						// FRAME_SAIDA
				tracker_vector[i].last_frame - tracker_vector[i].first_frame << ";" << // FRAMES_QUE_FICOU
				static_cast<float>(tracker_vector[i].first_frame) / fps << ";" <<			// TEMPO_ENTRADA
				static_cast<float>(tracker_vector[i].last_frame) / fps << ";" <<							// TEMPO_SAIDA
				static_cast<float>(tracker_vector[i].last_frame) / fps - static_cast<float>(tracker_vector[i].first_frame) / fps << ";" << // TEMPO_QUE_FICOU
				tracker_vector[i].bbox.x << ";" <<		// POSICAO_X
				tracker_vector[i].bbox.y << ";" <<		// POSICAO_Y
				tracker_vector[i].bbox.width << ";" <<	// LARGURA
				tracker_vector[i].bbox.height << ";" << // ALTURA
				tracker_vector[i].bbox.area() << "\n";  // AREA
			//img_output_string = "Imagens_Objetos\\objeto_ID_" + to_string(tracker_vector[i].id) + "_Saida.png";
			//imwrite(img_output_string, Mat(frame_copy, tracker_vector[i].prev_bbox[0]));
		}
	}

	cout << "Quantidade de objetos detectados: " << objects_counter << endl;
	out_log << "Quantidade de objetos detectados: " << objects_counter << "\n";
	//csv_file << "Total Objetos;" << objects_counter << "\n";

	csv_file.close();
	out_log.close();
	inputVideo.release();
	//output_video.release();
	end_func = clock();
	time_taken = double(end_func - start_func) / double(CLOCKS_PER_SEC);
	cout << "Fim do loop. Tempo: " << time_taken << endl;
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