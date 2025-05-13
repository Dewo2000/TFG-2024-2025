#include "Overlap.h"

#include <fstream>
#include <iostream>
#include "OCR.h"
//#include <opencv2/opencv.hpp>
//using namespace cv;
using namespace std;
Overlap::Overlap()
{
	_butMinW = 0;
	_butMinH = 0;
	_testPass = true;
}

Overlap::~Overlap()
{

}

void Overlap::test(const std::string& testString)
{
	_testPass = true;
	_boxes = _ocr->getBoundingBoxes(_imageUrl);
	_buttons = _ocr->getButtonsFromImage(_imageUrl, _butMinW, _butMinH);
	_testPass= !checkOverlap();
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

	return true;
}

void Overlap::setMinButtonSize(int w, int h)
{
	_butMinH = h;
	_butMinW = w;
}
