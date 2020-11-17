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
	
	//setUseOptimized(true);
	//setNumThreads(4);
	//ReadVideo videozin("Okeanos_Explorer_ROV_Dive_July_08_2019_Bottom_Hour_1.mp4");
	//ReadVideo videozin("Okeanos_Cortado.mp4");
	//ReadVideo videozin("Okeanos_Cortado2.mp4");
	//ReadVideo videozin("Okeanos_Cortado.mp4");
	ReadVideo videozin("Okeanos_Cortado4.mp4");
	//ideozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\Okeanos_Cortado4.mp4", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\Okeanos_Explorer_Video Bite _Deep_Sea_Shark_Makes_an_Appearance.mp4", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\IATAP_LEG01_6K1338_HIGH01_090.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\IATAP_LEG01_6K1338_HIGH02_097.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH02_098.M2T", 30, 30, 30, 4);
	videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH01_072.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH01_095.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH02_052.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH02_072.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Documents\\Faculdade\\2020\\2020-2\\Projeto\\Videos\\\IATAP_LEG01_6K1338_HIGH02_090.M2T", 30, 30, 30, 4);
	//videozin.read_video("C:\\Users\\Mathias\\Downloads\\Videos\\IATAP_LEG01_6K1338_HIGH01_072_Cortado.mp4", 30, 30, 30);
	//videozin.read_video("C:\\Users\\Mathias\\Downloads\\Videos\\video.mp4", 30, 30, 30);
	//videozin.read_video("C:\\Users\\Mathias\\Downloads\\Videos\\IATAP_LEG01_6K1338_HIGH01_072_Cortado.mp4", 30, 30, 30);
	//videozin.read_video("C:\\Users\\Mathias\\Downloads\\Videos\\IATAP_LEG01_6K1338_HIGH01_072_Cortado.mp4", 30, 30, 30);
	//videozin.read_video("Okeanos_Explorer_Vazio_Cortado.mp4");
	//videozin.read_video("Okeanos_Explorer_Video Bite _Deep_Sea_Shark_Makes_an_Appearance.mp4");

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
