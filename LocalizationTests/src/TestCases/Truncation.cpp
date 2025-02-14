#include "Truncation.h"
#include <iostream>
#include <sstream>
using namespace std;

Truncation::Truncation(std::string exp)
{
	_expected = exp;
}

Truncation::~Truncation()
{
}

void Truncation::test(const std::string& testString)
{

    std::vector<std::string> palabrasEsperado = split(_expected);
    std::vector<std::string> palabrasEntrada = split(testString);


    for (int i = 0; i < palabrasEntrada.size(); i++) {
        for (int j = 0; j < palabrasEsperado.size(); j++) {
            if (palabrasEsperado[j] == palabrasEntrada[i]) {
                std::cout << "El texto de entrada NO está truncado.\n";
                palabrasEsperado.erase(palabrasEsperado.begin());
                j--;
                break;
            }
            else if (palabrasEsperado[j].substr(0, palabrasEntrada[i].size()) == palabrasEntrada[i]) {
                std::cout << "El texto de entrada está truncado " << palabrasEsperado[j] << " -> " << palabrasEntrada[i] <<"\n";
                palabrasEsperado.erase(palabrasEsperado.begin());
                j--;
                break;
            }
        }

    }

    //if (testString == _expected) {
    //    std::cout << "El texto de entrada NO está truncado.\n";
    //}
    //if (testString.size() < _expected.size() && _expected.substr(0, testString.size()) == testString) {
    //    std::cout << "El texto de entrada está truncado.\n";
    //}
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
