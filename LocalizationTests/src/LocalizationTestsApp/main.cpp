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

int main(int argc, char* argv[]) {

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
	test->init(testCases[1]);
	test->run();
	test->release();
	delete test;
	return 0;
}


