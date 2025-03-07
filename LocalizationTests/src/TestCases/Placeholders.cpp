#include "Placeholders.h"
#include <iostream>
#include <regex>
using namespace std;

Placeholders::Placeholders(Delimitadores delimitador)
{
	_delimitador = delimitador;
}

Placeholders::~Placeholders()
{
}

void Placeholders::test(const std::string& testString)
{
    std::vector<PlaceholderResult> result;

    for (int i = 0; i < _delimitador.size();i++) {
        char inicio = _delimitador[i].first;
        char fin = _delimitador[i].second;
        std::string pattern = "\\" + std::string(1, inicio) + "(.*?)" + "\\" + std::string(1, fin);
        std::regex regexPlaceholder(pattern);
        std::sregex_iterator iter(testString.begin(), testString.end(), regexPlaceholder);
        std::sregex_iterator end;
        while (iter != end) {
            result.push_back({ static_cast<int>(iter->position()), iter->str() });
            ++iter;
        }
    }
    _result = result;
    for (const auto& p : result) {
        
        std::string s = "Posicion: " + std::to_string(p.posicion) + ", Placeholder: " + p.contenido + "\n";
        spdlog::warn(s);
    }
}
