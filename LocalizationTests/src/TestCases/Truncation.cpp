#include "Truncation.h"
#include <iostream>
#include <sstream>
using namespace std;

Truncation::Truncation(std::string exp)
{
	_expected = exp;
    _testPass = true;
}

Truncation::~Truncation()
{
}

void Truncation::test(const std::string& testString)
{
    _testPass = true;
    if (testString.length() == _expected.length())return;
   

    std::vector<std::string> palabrasEsperado = split(_expected);
    std::vector<std::string> palabrasEntrada = split(testString);


    for (int i = 0; i < palabrasEntrada.size(); i++) {
        for (int j = 0; j < palabrasEsperado.size(); j++) {
            if (palabrasEsperado[j] == palabrasEntrada[i]) {
                palabrasEsperado.erase(palabrasEsperado.begin());
                break;
            }
            else if (palabrasEsperado[j].substr(0, palabrasEntrada[i].size()) == palabrasEntrada[i]) {
                string s = "Palabra " + to_string(i) +" truncado " + palabrasEsperado[j] + " -> " + palabrasEntrada[i] + "\n";
                //spdlog::warn(s);
                palabrasEsperado.erase(palabrasEsperado.begin());
                _testPass = false;
                break;
            }
        }

    }
    if (palabrasEsperado.size() > 0) {
        //spdlog::warn("Falta palabras");
        _testPass = false;
    }
}

std::vector<std::string> Truncation::split(const std::string& texto)
{
    std::vector<std::string> palabras;
    std::istringstream stream(texto);
    std::string palabra;

    while (stream >> palabra) {
        palabras.push_back(palabra);
    }
    return palabras;
}
