#include "Placeholders.h"
#include <iostream>
#include <regex>
#include <codecvt>
#include <locale>
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
    std::u32string testString32=utf8_to_utf32(testString);
    for (int i = 0; i < _delimitador.size();i++) {
        char32_t inicio = _delimitador[i].first;
        char32_t fin = _delimitador[i].second;

        // Expresión regular en `std::u32string`
        std::u32string pattern = U"\\" + std::u32string(1, inicio) + U"(.*?)" + U"\\" + std::u32string(1, fin);
        std::wregex regexPlaceholder(reinterpret_cast<const wchar_t*>(pattern.c_str()));

        std::wstring textoW(reinterpret_cast<const wchar_t*>(testString32.c_str()));
        std::wsmatch match;

        std::wstring::const_iterator searchStart(textoW.cbegin());

        while (std::regex_search(searchStart, textoW.cend(), match, regexPlaceholder)) {
            std::u32string encontrado(reinterpret_cast<const char32_t*>(match.str().c_str()));
            size_t posicion = match.position() + (searchStart - textoW.begin());

            result.push_back({ static_cast<int>(posicion), encontrado });

            searchStart += match.position() + match.length(); // Mover iterador
        }

        //Versión normal, problema de codificación con utf8(ocupa más de un byte para letras con tilde)
   /*   char inicio = _delimitador[i].first;
        char fin = _delimitador[i].second;
        std::string pattern = "\\" + std::string(1, inicio) + "(.*?)" + "\\" + std::string(1, fin);
        std::regex regexPlaceholder(pattern,std::regex_constants::ECMAScript);
        std::sregex_iterator iter(testString.begin(), testString.end(), regexPlaceholder);
        std::sregex_iterator end;
        while (iter != end) {
            result.push_back({ static_cast<int>(iter->position(0)), iter->str() });
            ++iter;
        }*/
    }
    _result = result;
    for (const auto& p : result) {
        
        std::string s = "Posicion: " + std::to_string(p.posicion) + ", Placeholder: " + utf32_to_utf8(p.contenido) + "\n";
        spdlog::warn(s);
    }
}

std::u32string Placeholders::utf8_to_utf32(const std::string& input)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.from_bytes(input);
}

std::string Placeholders::utf32_to_utf8(const std::u32string& input)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(input);
}
