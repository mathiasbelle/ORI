// ORI: Organism Idendentifier
// Funcao main do programa, com uma simples interface, apenas para receber os parametros necessarios

#include <iostream>
#include "ReadVideo.h"

using namespace cv;
using namespace std;

int main() {

	ReadVideo video;
	string video_name = "";
	int min_time = 0;
	int min_width;
	int min_height;
	int frames_to_skip = 0;
	string answr = "";
	while (true) {
		cout << "Nome do video: ";
		cin >> video_name;
		cout << "Tempo minimo no rastreador: ";
		cin >> min_time;
		cout << "Largura minima: ";
		cin >> min_width;
		cout << "Altura minima: ";
		cin >> min_height;
		cout << "Quantidade de quadros para pular: ";
		cin >> frames_to_skip;

		if (video.read_video(video_name, min_time, min_width, min_height, frames_to_skip, true)) {
			cout << "Sucesso" << endl;
		} else {
			cout << "Algo deu errado" << endl;
		}
		cout << "Realizar o processo novamente? (s,n): ";
		cin >> answr;
		if (answr == "n" || answr == "na" || answr == "nao")
			break;
	}

	return 0;
}