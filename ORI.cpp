// ORI.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/video/tracking.hpp>
#include <iostream>
#include "ReadVideo.h"

using namespace cv;
using namespace std;

int main() {
	//ReadVideo videozin("C:\\Users\\Mathias\\Videos\\video.mp4");
	//cout << CV_VERSION;
	//ReadVideo videozin("Okeanos_Explorer_ROV_Dive_July_08_2019_Bottom_Hour_1.mp4");
	ReadVideo videozin("Okeanos_Cortado.mp4");

	return 0;
}
// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
