#include "Overlap.h"

#include <fstream>
#include <iostream>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

Overlap::Overlap()
{
	
}

Overlap::~Overlap()
{
	_ocr->End();
}

void Overlap::test(const std::string& testString)
{

}

bool Overlap::checkOverlap(ButtonLimits lim)
{
	int btnX = lim.x;
	int btnY=lim.y;
	int btnW=lim.w;
	int btnH=lim.h;
	// Limites del boton
	int btnRight = btnX + btnW;
	int btnBottom = btnY + btnH;

	// Verificar si el bounding box se sale de los l�mites del bot�n
	for (int i = 0;i < _boxes.size();i++) {
		if (_boxes[i].x < btnX || _boxes[i].x2 > btnRight || _boxes[i].y < btnY || _boxes[i].y2 > btnBottom) {
			return true;  // El bounding box se sale de los l�mites
		}
	}
	return false;  // El bounding box est� dentro de los l�mites
}

bool Overlap::Init(std::string imageUrl)
{
	//Esto tiene que ir en tesseract
	_ocr = new tesseract::TessBaseAPI();
	if (_ocr->Init("/home/trainingFont/trainedModel", "CourierPrime")) {
		std::cout << "fallo Init tess" << std::endl;
		return false;
	}
	//----------------------------------------------------------------------
	Pix* image = pixRead(imageUrl.c_str());
	if (!image) {
		std::cout << "fallo img" << std::endl;
		return false;
	}
	_ocr->SetImage(image);
	int result = _ocr->Recognize(0);
	if (result != 0) {
		std::cout << "Recognition failed with error code: " << result << std::endl;
		return false;
	}
	tesseract::ResultIterator* ri = _ocr->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
	if (ri != nullptr) {
		const char* word = ri->GetUTF8Text(level);
		while (word != nullptr) {
			float conf = ri->Confidence(level);
			int x1, y1, x2, y2;
			ri->BoundingBox(level, &x1, &y1, &x2, &y2);
			std::cout << "Word: '" << word << "' at (" << x1 << ", " << y1 << ") -> ("
				<< x2 << ", " << y2 << "), Confidence: " << conf << "\n";
			_boxes.push_back({ x1, y1, x2, y2 });
			delete[] word;

			// Avanza al siguiente nivel de iteraci�n
			if (!ri->Next(level)) {
				break;
			}
			word = ri->GetUTF8Text(level);
		}
	}
	pixDestroy(&image);
	return true;
}

void Overlap::getButtons()
{
	//Mat imagen = imread("ruta/a/imagen.jpg");

	//if (imagen.empty()) {
	//	cout << "No se pudo cargar la imagen." << endl;
	//	return -1;
	//}

	//// Convertir a escala de grises
	//Mat gris;
	//cvtColor(imagen, gris, COLOR_BGR2GRAY);

	//// Aplicar el detector de bordes
	//Mat bordes;
	//Canny(gris, bordes, 100, 200);

	//// Encontrar contornos
	//vector<vector<Point>> contornos;
	//findContours(bordes, contornos, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//// Dibujar los contornos que podr�an ser botones
	//for (size_t i = 0; i < contornos.size(); i++) {
	//	vector<Point> aprox;
	//	approxPolyDP(contornos[i], aprox, arcLength(contornos[i], true) * 0.02, true);

	//	if (aprox.size() == 4) { // Consideramos que un bot�n podr�a ser rectangular
	//		Rect rect = boundingRect(aprox);
	//		rectangle(imagen, rect, Scalar(0, 255, 0), 2);  // Dibujar el rect�ngulo alrededor del bot�n
	//		cout << "Coordenadas del bot�n: " << rect.x << ", " << rect.y
	//			<< ", ancho: " << rect.width << ", alto: " << rect.height << endl;
	//	}
	//}

	//// Mostrar la imagen con el bot�n detectado
	//imshow("Imagen con el bot�n detectado", imagen);
	//waitKey(0);
}
