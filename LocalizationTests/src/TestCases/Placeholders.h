#pragma once
#ifndef PLACEHOLDERS_H
#define PLACEHOLDERS_H

#include "TestCase.h"
#include <iostream>
#include <vector>
#include <string>
struct PlaceholderResult {
    int posicion;
    std::u32string contenido;
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

    std::u32string utf8_to_utf32(const std::string& input);

    // Función para convertir `std::u32string` (UTF-32) a `std::string` (UTF-8) para impresión
    std::string utf32_to_utf8(const std::u32string& input);
};
#endif


