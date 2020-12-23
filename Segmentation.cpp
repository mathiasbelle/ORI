#include "Segmentation.h"

Segmentation::Segmentation() {

}

vector<Rect2d> Segmentation::selective_search(Mat frame, int k) {

	clock_t start, end;
	start = clock();

	// Cria a busca seletiva
	Ptr<SelectiveSearchSegmentation> selective_search = createSelectiveSearchSegmentation();
	// Set input image
	selective_search->setBaseImage(frame);
	// Single strategy, gera menos resultados, porem mais relevantes
	selective_search->switchToSingleStrategy(k);

	// Cria um vector de Rect para armazenar os rects que a busca seletiva encontra
	vector<Rect> rects;
	selective_search->process(rects);
	std::cout << "Numero de regioes: " << rects.size() << std::endl;
	// Ordena o vetor, para que os rects maiores fiquem primeiro
	sort(rects.begin(), rects.end(), [](const Rect& left, const Rect& right) {
		return left.area() > right.area();
		}
	);

	//Mat im_out = frame.clone();
	//for (int i = 0; i < rects.size(); i++) {
	//	rectangle(im_out, rects[i], Scalar(0, 255, 0));
	//}
	//imwrite("Segmentacao_Original.png", im_out);

	// Remove o rect que engloba a imagem inteira
	rects.erase(rects.begin());

	for (int i = 0; i < rects.size(); i++) {
		for (int j = i + 1; j < rects.size(); j++) {
			//cout << "i = " << i << ", j = " << j << ", resultado: " << ((rects[i] & rects[j]).area() > 0) << endl;
			// Caso haja interseccao
			if ((rects[i] & rects[j]).area() > 0) {
				rects[i] = (rects[i] | rects[j]);
				rects.erase(rects.begin() + j);
				j--;
			}
		}
	}

	//im_out = frame.clone();
	//for (int i = 0; i < rects.size(); i++) {
	//	rectangle(im_out, rects[i], Scalar(0, 255, 0));
	//}
	//imwrite("Segmentacao_Sem_Interpolacao.png", im_out);


	vector < Rect2d> rects_2d;

	for (int i = 0; i < rects.size(); i++) {
		rects_2d.push_back(Rect2d(rects[i].x, rects[i].y, rects[i].width, rects[i].height));
	}


	//Mat im_out = frame.clone();

	//for (int i = 0; i < rects.size(); i++) {
	//	rectangle(im_out, rects[i], Scalar(0, 255, 0));
	//}
	//imwrite("imagem.png", im_out);
	end = clock();
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	//cout << "Tempo da segmentacao: " << fixed << time_taken << setprecision(5) << endl;
	return rects_2d;
}