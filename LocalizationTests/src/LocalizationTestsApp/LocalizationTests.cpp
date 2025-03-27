#include "LocalizationTests.h"
#include "Tesseract.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "Overlap.h"
#include "Truncation.h"
#include "Placeholders.h"

using json = nlohmann::json;
namespace fs = std::filesystem;
LocalizationTests::LocalizationTests() : state_(State::UNINITIALIZED), initTime_()
{

}
LocalizationTests::~LocalizationTests()
{
}

bool LocalizationTests::init(const std::vector<std::string>& args)
{
	if (parseArguments(args)) {
		
		return true;
	}
	return false;
}

//TODO: ¿Refactorizar el estado a un patrón Strategy/State?
void LocalizationTests::run()
{
	switch (state_) {
		case State::TRAINING:
		{
			Tesseract tess = Tesseract();
			if (tess.trainModel(_trainInfo.lan, _trainInfo.font, _trainInfo.iter,_trainInfo.clear)) {
				std::cout << "yes\n";
			}
			else std::cout << "no\n";
			break;
		}
		case State::TESTING:
		{
			if (initTesting()) {
			}
			break;
		}
		default:
			break;
	}
}

void LocalizationTests::release()
{
	//Liberación de ocr
	if(_ocr!=nullptr)
	_ocr->release();
	delete _ocr;
}

bool LocalizationTests::parseArguments(const std::vector<std::string>& args)
{
	if (args.size() < 2) return false;

	if (args[1] == "--test") {
		state_ = State::TESTING;
		if (args.size() >= 4 && args[2] == "-c") { //Si se proporciona un archivo de config
			_configFile = args[3];
		}
		getConfig();
	}
	else if (args[1] == "--train") {
		if (args.size() < 6 || args[2] != "-f" || args[4] != "-l") {
			std::cerr << "Uso incorrecto: --train -f <font> -l <language>\n";
			return false;
		}
		state_ = State::TRAINING;
		_trainInfo.font = args[3];
		_trainInfo.lan = args[5];
		// Buscar parámetros opcionales (-i iter, --clear)
		for (int i = 6; i < args.size(); ++i) {
			if (args[i] == "-i" && i + 1 < args.size()) {
				try {
					_trainInfo.iter = std::stoi(args[i + 1]);  // Convertir a entero
					i++;  // Saltar el siguiente argumento
				}
				catch (...) {
					std::cerr << "Error: Iteraciones debe ser un número entero.\n";
					return false;
				}
			}
			else if (args[i] == "--clear") {
				_trainInfo.clear = true;
			}
			else {
				std::cerr << "Argumento desconocido: " << args[i] << "\n";
				return false;
			}
		}
	}
	else {
		usage();
		return false;
	}

	return state_ != State::UNINITIALIZED;
}

void LocalizationTests::printTime()
{
	using namespace std::chrono;
	duration<double> dur = duration_cast<nanoseconds>(system_clock::now() - initTime_);

	std::cout << "Test completed in " << dur.count() << " nanoseconds" << std::endl;

	initTime_ = system_clock::now();
}

void LocalizationTests::usage() {
	std::cout << "Usage: LocalizationTests Option\n" << "Options: --train for training the tesseract OCR --test for testing\n"
		<< "  --test [-c <configfile>]\n"
		<< "  --train -f <font> -l <language> [-i <iteraciones>] [--clear]\n";
}

bool LocalizationTests::getConfig()
{
	try {
		std::ifstream archivo;
		if (_configFile != "") {
			archivo.open(_configFile);
		}
		else {
			archivo.open("config.json");
		}
		
		if (!archivo) {
			throw std::runtime_error("No se puede o no existe el archivo de configuracion");
			return false;
		}

		// Leer el JSON
		json config;
		archivo >> config;
		archivo.close();

		// Acceder a los valores
		_configInfo.imgPath = config["imagePath"];
		_configInfo.gtPath = config["gtPath"];
		_configInfo.model = config["model"];
		_configInfo.modelPath = config["modelPath"];
		_configInfo.outputPath = config["outputPath"];
		_configInfo.ocr = config["OCR"];

		for (const auto& s : config["placeholders"]) {
			if (!s.is_string() || s.get<std::string>().size() != 1) {
				throw std::runtime_error("Error en simbolos_placeholders: Deben ser caracteres individuales.");
			}
			_configInfo.placeholders.push_back({s.get<std::string>()[0], s.get<std::string>()[0]
		});
		}

		return true;
	}
	catch (const std::exception& e) {
		
		spdlog::error(e.what());
		return false;
	}
}

bool LocalizationTests::initTesting()
{
	//Seleccion de ocr;
	if (_configInfo.ocr == "Tesseract") {
		_ocr = new Tesseract();
	}

	//Inicialización de OCR
	if (!_ocr->init(_configInfo.modelPath, _configInfo.model))return false;
	

	//Si el directorio imagen existe
	if (!fs::exists(_configInfo.imgPath) || !fs::is_directory(_configInfo.imgPath)) {
		std::cerr << "El directorio no existe o no es válido.\n";
		return false;
	}
	//Reconocimiento de los textos de las imagenes
	_ocr->getDirImgText(_configInfo.imgPath, _configInfo.outputPath + "/recognition",_configInfo.gtPath);

	json jsonResult;
	//Correr por el directorio y buscar imagenes
	for (const auto& entry : fs::directory_iterator(_configInfo.imgPath)) {
		if (entry.is_regular_file()) {
			std::string extension = entry.path().extension().string();
			std::string nombre_imagen = entry.path().stem().string();  // Nombre sin extensión
			std::string archivo_gt = _configInfo.gtPath + nombre_imagen + "-gt.txt";
			std::string archivo_recog = _configInfo.outputPath+"recognition/" + nombre_imagen + ".txt";

			// Convertir extensión a minúsculas (por si acaso)
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
			if (extension == ".jpg" || extension == ".png" || extension == ".jpeg") {

				std::string rutaImagen = entry.path().string();
				std::cout << "Procesando: " << rutaImagen << std::endl;
				std::string gt = "";
				std::string recog = "";

				//Busqueda del ground-truth de la imagen
				if (fs::exists(archivo_gt)) {
					std::ifstream archivo(archivo_gt);
					if (!archivo) {
						std::cerr << "No se pudo abrir el archivo TXT: " << archivo_gt << std::endl;
						continue;
					}
					else {
						gt.assign((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
					}
				}

				//Busqueda de la salida de la imagen
				if (fs::exists(archivo_recog)) {
					std::ifstream archivo(archivo_recog);
					if (!archivo) {
						std::cerr << "No se pudo abrir el archivo TXT: " << archivo_recog << std::endl;
						continue;
					}
					else {
						recog.assign((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
					}
				}

				json imageResult;
				imageResult["texto_esperado"] = gt;
				imageResult["texto_reconocido"] = recog;

				testAll(rutaImagen, gt, recog,imageResult);
				jsonResult[entry.path().filename()] = imageResult;
				

			}
			
		}
	}
	std::string command = "sudo chmod 777 " + _configInfo.outputPath;
	std::system(command.c_str());
	// Escribir el objeto JSON en un archivo
	std::ofstream archivo(_configInfo.outputPath + "result.json");
	if (archivo.is_open()) {
		archivo << jsonResult.dump(4);  // 'dump(4)' para escribir el JSON con formato bonito (indentación de 4 espacios)
		archivo.close();
		std::cout << "JSON guardado exitosamente en 'result.json'" << std::endl;
	}
	else {
		std::cerr << "No se pudo abrir el archivo para escribir" << std::endl;
	}
	
	return true;
}

void LocalizationTests::testAll(std::string img, std::string gt,std::string recog,json& imageResult)
{
	json tests;

	Overlap lap = Overlap();
	json overlap;
	lap.Init(img, _ocr);
	lap.test();
	if (lap.getPass()) overlap["test_pass"] = true;
	else overlap["test_pass"] = false;

	Truncation trun = Truncation(gt);
	json truncation;
	trun.test(recog);
	if (trun.getPass())truncation["test_pass"] = true;
	else truncation["test_pass"] = false;

	Placeholders holder = Placeholders(_configInfo.placeholders);
	json holders;
	holder.test(recog);
	if (holder.getPass())holders["test_pass"] = true;
	else {
		holders["test_pass"] = false;
		std::vector<PlaceholderResult> result = holder.getResult();
		json error;
		for (int i = 0;i < result.size();i++) {
			json presult;
			presult["pos"] = result[i].posicion;
			presult["contenido"] = result[i].contenido;
			error.push_back(presult);
		}
		holders["errors"] = error;
	}
	tests["overlap"] = overlap;
	tests["truncamiento"] = truncation;
	tests["placeholders"] = holders;
	imageResult["tests"] = tests;

	
}
