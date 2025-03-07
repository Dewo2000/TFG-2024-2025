#pragma once
#ifndef PLACEHOLDERS_H
#define PLACEHOLDERS_H

#include "TestCase.h"
#include <iostream>
#include <vector>
#include <string>
struct PlaceholderResult {
    int posicion;
    std::wstring contenido;
};
typedef std::vector<std::pair<char, char>> Delimitadores;
class Placeholders : TestCase {
public:
	Placeholders(Delimitadores delimitador);
	~Placeholders();
	void test(const std::string& testString) override;
private:
    Delimitadores _delimitador;
	std::vector<PlaceholderResult> _result;

    std::wstring utf8_to_wstring(const std::string& input);

	std::string wstring_to_utf8(const std::wstring& wstr);
};
#endif


