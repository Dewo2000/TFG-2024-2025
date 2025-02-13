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



using json = nlohmann::json;

int main(int argc, char *argv[]) {



	
	spdlog::info("Welcome to spdlog!");
	spdlog::error("Some error message with arg: {}", 1);
	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	spdlog::set_level(spdlog::level::debug); // Set global log level to debug
	spdlog::debug("This message should be displayed..");
	try
	{
		auto logger = spdlog::basic_logger_mt("basic_logger", "basic-log.txt");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
	}
	spdlog::get("basic_logger")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

	//Para testear paso de argumentos
	std::vector<std::vector<std::string>> testCases = {
		{"program", "--test"},
		{"program", "--test", "-c", "/home/localizationtests/volumen/config.json"},
		{"program", "--train", "-f", "Arial", "-l", "eng"},
		{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "50"},
		{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "100", "--clear"},
		{"program", "--train", "-f", "Arial", "-l", "eng", "--clear"},
		{"program", "--train", "-f", "Arial", "-l", "eng", "-i", "abc"} // Caso con error
	};

	LocalizationTests* test = new LocalizationTests();
	test->init(testCases[0]);
	test->run();
	test->release();
	delete test;

	std::string volumen = "/home/localizationtests/volumen/";
	OCR* ocr = new Tesseract();
	ocr->init("/home/trainingFont/trainedModel", "eng");
	//ocr->getDirImgText("/home/images", "/home/result");
	ocr->release();
	delete ocr;
	return 0;
}
