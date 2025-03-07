#include "Overlap.h"

#include <fstream>
#include <iostream>
#include "OCR.h"
//#include <opencv2/opencv.hpp>
//using namespace cv;
using namespace std;
Overlap::Overlap()
{
	_butMinW = 65;
	_butMinH = 30;
}

Overlap::~Overlap()
{

}

void Overlap::test(const std::string& testString)
{
	_result = checkOverlap();
}

bool Overlap::checkOverlap()
{
	for (int j = 0; j < _buttons.size(); j++) {
		int btnX = _buttons[j].x;
		int btnY = _buttons[j].y;
		int btnW = _buttons[j].w;
		int btnH = _buttons[j].h;
		// Limites del boton
		int btnRight = btnX + btnW;
		int btnBottom = btnY + btnH;

		// Verificar si el bounding box se sale de los límites del botón
		for (int i = 0; i < _boxes.size(); i++) {
			// Si la esquina superior izquierda está dentro del botón
			if(_boxes[i].x > btnX && _boxes[i].x < btnX + btnW && _boxes[i].y > btnY && _boxes[i].y < btnY + btnH) 
			{
				// Si se sale a la derecha o hacia abajo
				if (_boxes[i].x2 > btnRight || _boxes[i].y2 > btnBottom) { 
					return true;
				}
			}
			// La esquina superior izquierda no está en el botón por lo que se ignora

		}
	}
	return false;  // El bounding box está dentro de los límites
}

bool Overlap::Init(std::string imageUrl, OCR* ocr)
{
	_imageUrl = imageUrl;
	_ocr = ocr;
	_boxes = _ocr->getBoundingBoxes(imageUrl);
	return true;
}

bool Overlap::getButtons()
{
	//Mat imagen = imread(_imageUrl);

	//if (imagen.empty()) {
	//	cout << "No se pudo cargar la imagen." << endl;
	//	return -1;
	//}

	//cv::Mat gray;
	//cv::cvtColor(imagen, gray, cv::COLOR_BGR2GRAY);

	//// Aplicar un umbral para binarizar la imagen
	//cv::Mat thresh;
	//cv::threshold(gray, thresh, 200, 255, cv::THRESH_BINARY_INV); // Ajusta el valor 200 según sea necesario

	//// Encontrar contornos
	//std::vector<std::vector<cv::Point>> contornos;
	//cv::findContours(thresh, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//// Dibujar los contornos que podrían ser botones
	//for (size_t i = 0; i < contornos.size(); i++) {
	//	vector<Point> aprox;
	//	approxPolyDP(contornos[i], aprox, arcLength(contornos[i], true) * 0.02, true);

	//	if (aprox.size() == 4) { // Consideramos que un botón podría ser rectangular
	//		Rect rect = boundingRect(aprox);
	//		rectangle(imagen, rect, Scalar(0, 255, 0), 2);  // Dibujar el rectángulo alrededor del botón
	//		//Solo tenemos en cuenta los botones de un tamaño mínimo
	//		if (rect.width >= _butMinW && rect.height >= _butMinH) {
	//			cout << "Coordenadas del boton: " << rect.x << ", " << rect.y
	//				<< ", ancho: " << rect.width << ", alto: " << rect.height << endl;
	//			_buttons.push_back({ rect.x,rect.y,rect.width,rect.height });
	//		}
	//		
	//	}
	//}
	return true;
}

void Overlap::setMinButtonSize(int w, int h)
{
	_butMinH = h;
	_butMinW = w;
}
