#include "Placeholders.h"
#include <iostream>
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
        int pos = 0;
        while ((pos = testString.find(_delimitador[i].first,pos)) != std::string::npos) {
            size_t endPos = testString.find(_delimitador[i].second, pos + 1);
            if (endPos != std::string::npos) {
                std::string contenido = testString.substr(pos, endPos - pos + 1);
                result.push_back({ pos, contenido });
                pos = endPos + 1;
            }
            else { //Ha llegado al final y no se encuentra
                break; 
            }
        }
    }
    _result = result;
    for (const auto& p : result) {
        
        std::string s = "Posicion: " + std::to_string(p.posicion) + ", Placeholder: " + p.contenido + "\n";
        spdlog::warn(s);
    }
}
