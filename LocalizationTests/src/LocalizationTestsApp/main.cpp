#include <iostream>
#include <string>
#include "Tesseract.h"
#include "LocalizationTests.h"
#include "Overlap.h"

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

#include "Placeholders.h"
#include "Truncation.h"
using json = nlohmann::json;
//TMP
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <cstdlib>
#include <opencv2/photo.hpp>
#include <opencv2/opencv.hpp>
int main(int argc, char *argv[]) {


	std::string volumen = "/home/localizationtests/volumen/";
		//Inicio tesseract (se necesita modelo e idioma)
	tesseract::TessBaseAPI* _ocr = new tesseract::TessBaseAPI();
	if (_ocr->Init("/home/trainingFont/trainedModel", "eng")) {
		std::cout << "fallo Init tess" << std::endl;
		return false;
	}

	//Carpetas de imagetesting
	std::vector<std::pair<std::string, int>> carpetas;
	carpetas.push_back({ "fondo_complejo", 5 });
	carpetas.push_back({ "fondo_simple", 5 });
	carpetas.push_back({ "texto_en_bocadillo", 8 });
	carpetas.push_back({ "pixelart", 9 });
	carpetas.push_back({ "texto_en_bocadillo_2", 5 });

	//Creación de carpetas de resultados
	std::string command = "sudo mkdir -p /home/result";
	std::system(command.c_str());
	command = "sudo chmod 777 /home/result";
	std::system(command.c_str());

	for (int j = 0;j < carpetas.size();j++) {
		std::string folder = carpetas[j].first;
		//Permisos de escritura de las carpetas
		std::string command = "sudo mkdir -p /home/result/" + folder;
		std::system(command.c_str());
		command = "sudo chmod 777 /home/result/" + folder;
		std::system(command.c_str());


		for (int i = 1;i <= carpetas[j].second;i++) {
			//Nombre de imagen y gt
			std::string iName = volumen + folder + "/" + std::to_string(i) + ".png";
			std::string gtName = volumen + folder + "/" + std::to_string(i) + "-gt.txt";
			 //Guardar el texto en un archivo en la carpeta result
			std::string savepath = "/home/result/" + folder;
			std::string imagesavepath = "/home/imageresult/" + folder;

			cv::Mat image = cv::imread(iName, cv::IMREAD_COLOR);

			if (image.empty()) {
				std::cerr << "Error al leer la imagen" << std::endl;
				return -1;
			}	
			_ocr->SetImage(image.data, image.cols, image.rows, 1, image.step);
			char* outText = _ocr->GetUTF8Text();

			std::ofstream outFile(savepath + "/" + std::to_string(i) + ".txt");
			if (outFile.is_open()) {
				outFile << outText;
				outFile.close();
				std::cout << "Texto guardado" << std::endl;
			}
			else {
				std::cerr << "No se pudo abrir el archivo para escribir" << std::endl;
			}

			 //Liberar recursos
			delete[] outText;
		}
	}
	
	
	_ocr->End();

	//std::string texto = "Texto con *placeholder1* y también [otroPlaceholder] y {unoMas}. Texto con *placeholder1* y también [otroPlaceholder] y {unoMas}.";
	//std::vector<std::pair<char, char>> delimitadores = {
	//	{'*', '*'},   // Placeholder entre *
	//	{'[', ']'},   // Placeholder entre []
	//	{'{', '}'},   // Placeholder entre {}
	//};

	//Placeholders h = Placeholders(delimitadores);
	//h.test(texto);

	//
	//
	//std::string esperado = "Hola este es un texto completo con varias palabras importantes";

	//// Casos de prueba
	//std::string entrada1 = "Hol est es un tex comple con var palab import"; // Varias truncadas
	//std::string entrada2 = "Hola este es un texto completo con varias palabras importantes."; // No truncado
	//std::string entrada3 = "Hola est es otro text"; // Diferente en algunas palabras
	//std::string entrada4 = "Hola este es"; // Truncado desde "un"

	//Truncation t = Truncation(esperado);
	//t.test(entrada1);
	////t.test(entrada2);
	////t.test(entrada3);
	////t.test(entrada4);

	////spdlog::info("Welcome to spdlog!");
	////spdlog::error("Some error message with arg: {}", 1);
	////spdlog::warn("Easy padding in numbers like {:08d}", 12);
	////spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	////spdlog::set_level(spdlog::level::debug); // Set global log level to debug
	////spdlog::debug("This message should be displayed..");
	////try
	////{
	////	auto logger = spdlog::basic_logger_mt("basic_logger", "basic-log.txt");
	////}
	////catch (const spdlog::spdlog_ex& ex)
	////{
	////	std::cout << "Log init failed: " << ex.what() << std::endl;
	////}
	////spdlog::get("basic_logger")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

	////Para testear paso de argumentos
	//std::vector<std::vector<std::string>> testCases = {
	//	{"program", "--test"},
	//	{"program", "--test", "-c", "/home/localizationtests/volumen/config.json"},
	//	{"program", "--train", "-f", "Arial", "-l", "eng"},
	//	{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "50"},
	//	{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "100", "--clear"},
	//	{"program", "--train", "-f", "Arial", "-l", "eng", "--clear"},
	//	{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "abc"} // Caso con error
	//};

	//LocalizationTests* test = new LocalizationTests();
	//test->init(testCases[0]);
	//test->run();
	//test->release();
	//delete test;


	//OCR* ocr = new Tesseract();
	//ocr->init("/home/trainingFont/trainedModel", "eng");
	////ocr->getDirImgText("/home/images", "/home/result");
	//ocr->release();
	//delete ocr;
	return 0;
}
