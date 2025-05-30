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
#include "Similarity.h"

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
void LocalizationTests::run()
{
	switch (state_) {
		case State::TRAINING:
		{
			//Crea la clase OCR y hace el entreno
			Tesseract tess = Tesseract();
			if (tess.trainModel(_trainInfo.lan, _trainInfo.font, _trainInfo.iter,_trainInfo.clear)) {
				std::cout << "yes\n";
			}
			else std::cout << "no\n";
			break;
		}
		case State::TESTING:
		{
			//Inicia el testing
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
	//Liberaci�n de ocr
	if(_ocr!=nullptr)
	_ocr->release();
	delete _ocr;
}

bool LocalizationTests::parseArguments(const std::vector<std::string>& args)
{
	if (args.size() < 2) {
		usage();
		return false;
	}

	//En el caso de test
	if (args[1] == "--test") {
		state_ = State::TESTING;
		if (args.size() >= 4 && args[2] == "-c") { //Si se proporciona un archivo de config
			_configFile = args[3];
		}
		getConfig();
	}
	//En el caso de entreno
	else if (args[1] == "--train") {
		if (args.size() < 6 || args[2] != "-f" || args[4] != "-l") {
			std::cerr << "Uso incorrecto: --train -f <font> -l <language>\n";
			return false;
		}
		state_ = State::TRAINING;
		_trainInfo.font = args[3];
		_trainInfo.lan = args[5];
		// Buscar par�metros opcionales (-i iter, --clear)
		for (int i = 6; i < args.size(); ++i) {
			if (args[i] == "-i" && i + 1 < args.size()) {
				try {
					_trainInfo.iter = std::stoi(args[i + 1]);  // Convertir a entero
					i++;  // Saltar el siguiente argumento
				}
				catch (...) {
					std::cerr << "Error: Iteraciones debe ser un n�mero entero.\n";
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
	std::cerr << "Usage: LocalizationTests Option\n" << "Options: --train for training the tesseract OCR --test for testing\n"
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
		_configInfo.placeholders.push_back({ config["placeholders"]["begin"], config["placeholders"]["end"] });
		

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
	else if (_configInfo.ocr == "") {
		_ocr = new Tesseract();
	}

	//Inicializaci�n de OCR
	if (!_ocr->init(_configInfo.modelPath, _configInfo.model))return false;
	

	//Si el directorio imagen existe
	if (!fs::exists(_configInfo.imgPath) || !fs::is_directory(_configInfo.imgPath)) {
		std::cerr << "El directorio no existe o no es v�lido.\n";
		return false;
	}
	//Reconocimiento de los textos de las imagenes
	_ocr->getDirImgText(_configInfo.imgPath, _configInfo.outputPath + "/recognition",_configInfo.gtPath);

	json jsonResult;
	//Correr por el directorio y buscar imagenes
	for (const auto& entry : fs::directory_iterator(_configInfo.imgPath)) {
		if (entry.is_regular_file()) {
			std::string extension = entry.path().extension().string();
			std::string nombre_imagen = entry.path().stem().string();  // Nombre sin extensi�n
			std::string archivo_gt = _configInfo.gtPath + nombre_imagen + "-gt.txt";
			std::string archivo_recog = _configInfo.outputPath+"recognition/" + nombre_imagen + ".txt";

			// Convertir extensi�n a min�sculas (por si acaso)
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
				if (!recog.empty() && recog.back() == '\n') {
					recog.pop_back();  // elimina el �ltimo car�cter
				}
				//Crea el json y guarda los textos
				json imageResult;
				imageResult["texto_esperado"] = gt;
				imageResult["texto_reconocido"] = recog;
				//Ejecuta el test en todo
				testAll(rutaImagen, gt, recog,imageResult);
				jsonResult[entry.path().filename()] = imageResult;
				

			}
			
		}
	}
	// Escribir el objeto JSON en un archivo
	std::string command = "sudo chmod 777 " + _configInfo.outputPath;
	std::system(command.c_str());
	std::ofstream archivo(_configInfo.outputPath + "result.json");
	if (archivo.is_open()) {
		archivo << jsonResult.dump(4); 
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
	//Test de similarity
	json tests;
	bool overallpass = true;
	Similarity sim = Similarity(gt);
	sim.test(recog);
	double simi=sim.getSimilarity();

	//Json de los tests
	json overlapJson;
	overlapJson["test_pass"] = true;
	json truncationJson;
	truncationJson["test_pass"] = true;
	json holdersJson;
	holdersJson["test_pass"] = true;
	
	//Test de solapamiento
	Overlap lap = Overlap();
	lap.Init(img, _ocr);
	lap.test();
	if (lap.getPass()) overlapJson["test_pass"] = true;
	else {
		overlapJson["test_pass"] = false;
	overallpass = false;
	}
	//Si son iguales las cadenas se salta los tests
	if (simi == 100.0) {
		tests["overlap"] = overlapJson;
		tests["truncamiento"] = truncationJson;
		tests["placeholders"] = holdersJson;
		tests["overall_pass"] = overallpass;
		tests["similarity"] = simi;
		imageResult["tests"] = tests;
		return;
	}
	//Test de truncamiento
	Truncation trun = Truncation(gt);
	
	trun.test(recog);
	if (trun.getPass())truncationJson["test_pass"] = true;
	else { truncationJson["test_pass"] = false; 
	overallpass = false;
	}
	//Test de placeholders
	Placeholders holder = Placeholders(_configInfo.placeholders);

	holder.test(recog);
	if (holder.getPass())holdersJson["test_pass"] = true;
	else {
		holdersJson["test_pass"] = false;
		overallpass = false;
		std::vector<PlaceholderResult> result = holder.getResult();
		json error;
		for (int i = 0;i < result.size();i++) {
			json presult;
			presult["pos"] = result[i].posicion;
			presult["contenido"] = result[i].contenido;
			error.push_back(presult);
		}
		holdersJson["errors"] = error;
	}
	//Escritura en json de los resultados
	tests["overlap"] = overlapJson;
	tests["truncamiento"] = truncationJson;
	tests["placeholders"] = holdersJson;
	tests["overall_pass"] = overallpass;
	tests["similarity"] = simi;
	imageResult["tests"] = tests;

	
}
