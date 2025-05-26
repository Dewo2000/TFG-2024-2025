#include "OCR.h"
#include <fstream>
#include <sstream>
#include "Levenshtein.h"
#include <opencv2/photo.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
std::string OCR::readGT(const std::string& rutaArchivo)
{
	std::ifstream archivo(rutaArchivo, std::ios::in | std::ios::binary);
	if (!archivo) {
		return "";
	}

	// Mover el puntero al final para determinar el tamaño
	archivo.seekg(0, std::ios::end);
	std::streamsize tamano = archivo.tellg();
	archivo.seekg(0, std::ios::beg);

	// Reservar espacio y leer todo el contenido
	std::string contenido(tamano, '\0');
	archivo.read(&contenido[0], tamano);
	archivo.close();

	return contenido;
}

std::string OCR::findMostSimilarLine(const std::string& target, const std::vector<std::string>& recognizedLines, double threshold)
{
	double maxSimilarity = 0.0;
	std::string bestMatch;
	//Obtiene la distancia de cada línea buscando la que de más similitud
	for (const auto& line : recognizedLines) {
		int distance = Levenshtein::levenshteinDist(target, line);
		double similarity = 1.0 - (double)distance / std::max(target.size(), line.size());
		if (similarity > maxSimilarity && similarity >= threshold) {
			maxSimilarity = similarity;
			bestMatch = line;
		}
	}

	return bestMatch;
}

std::vector<std::string> OCR::splitIntoLines(const std::string& text)
{
	std::vector<std::string> lines;
	std::istringstream stream(text);
	std::string line;
	while (std::getline(stream, line)) {
		lines.push_back(line);
	}
	return lines;
}

bool OCR::preprocessing(imageInfo& imageOutput, std::string imgPath, std::string imageName)
{
	string iName = imgPath + "/" + imageName;
	cv::Mat image;
	image = cv::imread(iName);
	if (image.empty()) {
		std::cerr << "Error al leer la imagen" << std::endl;
		return false;
	}
	//Convierte a grises
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	//Escala la imágen
	float factor = 1.5;
	cv::resize(image, image, cv::Size(int(image.cols * factor), int(image.rows * factor)), factor, factor, cv::INTER_LINEAR);

	//Aplica constraste
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(3.0, cv::Size(8, 8));
	clahe->apply(image, image);
	//Aplica simple threshold
	cv::threshold(image, image, 130, 255, cv::THRESH_BINARY);

	//Para obtener la imagen salida del preprocesado
	
	//string oPath = imgPath+"/"+"prepro";
	//std::string command = "sudo mkdir -p " + oPath;
	//std::system(command.c_str());
	//command = "sudo chmod 777 " + oPath;
	//std::system(command.c_str());
	//string oName = imageName;
	//size_t pos = oName.rfind(".png");
	//if (pos != std::string::npos) {
	//	oName.erase(pos, 4);  // ".png" tiene 4 caracteres
	//}
	//oName += "_prepro.png";

	//cv::imwrite(oPath +"/"+oName, image);

	imageOutput.cols = image.cols;
	imageOutput.rows = image.rows;
	imageOutput.step = image.step;

    // Reservar memoria y copiar los datos
	imageOutput.data = new unsigned char[imageOutput.step * imageOutput.rows];
    memcpy(imageOutput.data, image.data, imageOutput.step * imageOutput.rows);
	return true;
}
std::vector<ButtonLimits> OCR::getButtonsFromImage(std::string imgPath,int butMinW,int butMinH)
{
	std::vector<ButtonLimits> result;
	cv::Mat imagen = cv::imread(imgPath);

	if (imagen.empty()) {
		cout << "No se pudo cargar la imagen." << endl;
		return result;
	}

	cv::Mat gray;
	cv::cvtColor(imagen, gray, cv::COLOR_BGR2GRAY);

	cv::Mat edges;
	cv::Canny(gray, edges, 30, 150); 

	// Encontrar contornos
	std::vector<std::vector<cv::Point>> contornos;
	cv::findContours(edges, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Dibujar los contornos que podrían ser botones
	for (size_t i = 0; i < contornos.size(); i++) {
		vector<cv::Point> aprox;
		approxPolyDP(contornos[i], aprox, arcLength(contornos[i], true) * 0.02, true);

		if (aprox.size() == 4) { // Consideramos que un botón podría ser rectangular
			cv::Rect rect = boundingRect(aprox);
			rectangle(imagen, rect, cv::Scalar(0, 255, 0), 2);  // Dibujar el rectángulo alrededor del botón
			//Solo tenemos en cuenta los botones de un tamaño mínimo
			if (rect.width >= butMinW && rect.height >= butMinH) {
				result.push_back({ rect.x,rect.y,rect.width,rect.height });
			}
			
		}
	}
}