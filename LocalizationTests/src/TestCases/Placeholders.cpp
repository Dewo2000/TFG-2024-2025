#include "Placeholders.h"
#include <iostream>
#include <regex>
#include <codecvt>
#include <locale>
#include <unicode/unistr.h>  // ICU UnicodeString
#include <unicode/ustream.h> // Para mostrar UnicodeString
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
    std::wstring wstr =utf8_to_wstring(testString);
    std::string a = wstring_to_utf8(wstr);
    for (int i = 0; i < _delimitador.size();i++) {
        wchar_t inicio = _delimitador[i].first;
        wchar_t fin = _delimitador[i].second;
        // Expresión regular en `std::u32string`
       // Construir el patrón de expresión regular para este par de delimitadores
        std::wstring pattern = L"\\" + std::wstring(1, inicio) + L"(.*?)" +L"\\" + std::wstring(1, fin);

        // Construir la expresión regular

        std::wregex regexPlaceholder(pattern);

        std::wsmatch match;
        std::wstring::const_iterator searchStart(wstr.cbegin());

        while (std::regex_search(searchStart, wstr.cend(), match, regexPlaceholder)) {
            std::wstring encontrado(match.str());  // Capturar solo el contenido entre los delimitadores
            size_t posicion = match.position() + (searchStart - wstr.begin());

            result.push_back({ static_cast<int>(posicion), wstring_to_utf8(encontrado) });

            searchStart += match.position() + match.length(); // Mover iterador
        }
    }
    _result = result;
    for (const auto& p : result) {
        
        std::string s = "Posicion: " + std::to_string(p.posicion) + ", Placeholder: " + p.contenido + "\n";
        spdlog::warn(s);
    }
}


std::wstring Placeholders::utf8_to_wstring(const std::string& input)
{
    icu::UnicodeString unicodeStr = icu::UnicodeString::fromUTF8(input);
    std::wstring wStr;

    // Convertir UnicodeString a std::wstring
    int32_t length = unicodeStr.length();
    wStr.resize(length);

    for (int32_t i = 0; i < length; ++i) {
        wStr[i] = static_cast<wchar_t>(unicodeStr[i]);
    }

    return wStr;
}

std::string Placeholders::wstring_to_utf8(const std::wstring& wStr)
{

    icu::UnicodeString unicodeStr;

    // Convertir wchar_t (UTF-16) a UnicodeString
    for (size_t i = 0; i < wStr.size(); ++i) {
        unicodeStr.append(wStr[i]);
    }

    // Convertir UnicodeString a UTF-8
    std::string utf8Str;
    unicodeStr.toUTF8String(utf8Str);

    return utf8Str;
}
