#pragma once
#ifndef LOCALIZATIONTESTS_H
#define LOCALIZATIONTESTS_H

#include <chrono>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
//Información del archivo de configuración
struct ConfigInfo {
	std::string imgPath;
	std::string gtPath;
	std::string model;
	std::string modelPath;
	std::string outputPath;
	std::string ocr;
	std::vector<std::pair<std::string, std::string>> placeholders;
};
//Información de entreno
struct TrainInfo {
	std::string font;
	std::string lan;
	int iter;
	bool clear;
};
class OCR;
class LocalizationTests {
public:
	LocalizationTests();
	~LocalizationTests();
	bool init(const std::vector<std::string>& args);
	void run();
	void release();
private:
	enum class State {
		TESTING,
		TRAINING,
		UNINITIALIZED
	};
	State state_;
	std::chrono::system_clock::time_point initTime_;
	//Parsea los argumentos
	bool parseArguments(const std::vector<std::string>& args);
	void printTime();
	void usage();
	bool getConfig();
	TrainInfo _trainInfo;
	std::string _configFile;
	ConfigInfo _configInfo;
	bool initTesting();
	//Ejecuta todos los tests
	void testAll(std::string img,std::string gt,std::string recog, nlohmann::json &imageResult);
	OCR* _ocr;
};
#endif