#include "Truncation.h"
#include "Placeholders.h"
#include "OCR.h"
#include "Tesseract.h"
#include "Overlap.h"
#define CATCH_CONFIG_MAIN  // Esto define la función main() para las pruebas
#include "catch2/catch.hpp"      // Incluir Catch2

TEST_CASE("Placeholders", "[Placeholders]") {

	
	std::string texto = "Placeholders";
	std::vector<std::pair<std::string, std::string>> delimitadores = {
		{"%_", "_%"} 
	};
	Placeholders h = Placeholders(delimitadores);
	h.test(texto);
	REQUIRE(h.getPass() == true);
	h.test("Sin placeholder");
	REQUIRE(h.getPass() == true);
	h.test("%Sin placeholder%");
	REQUIRE(h.getPass() == true);
	h.test("%Soy un% monton de palabras y signos $&/ pero sin placeholders");
	REQUIRE(h.getPass() == true);
	h.test("%_casi&");
	REQUIRE(h.getPass() == true);
	h.test("_casi_&");
	REQUIRE(h.getPass() == true);
	h.test("&_casi_");
	REQUIRE(h.getPass() == true);
	h.test("&_casi&_");
	REQUIRE(h.getPass() == true);
}

TEST_CASE("PlaceholdersNegativo", "[Placeholders]") {
	std::string texto = "%_Placeholders_%";
	std::vector<std::pair<std::string, std::string>> delimitadores = {
		{"%_", "_%"}
	};
	Placeholders h = Placeholders(delimitadores);
	h.test(texto);
	std::vector<PlaceholderResult> expect({ { 0,"%_Placeholders_%" } });
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test(" %_Placeholders_%");
	expect={ { 1,"%_Placeholders_%" } };
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test("hola%_Placeholders_%");
	expect = { { 4,"%_Placeholders_%" } };
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test("%_Placeholders_%%_Placeholders_%");
	expect = { { 0,"%_Placeholders_%" }, { 16,"%_Placeholders_%" } };
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test({"%_BlablablaPlaceholders_%" });
	expect = { { 0,"%_BlablablaPlaceholders_%" } };
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
}

TEST_CASE("Overlap", "[Overlap]") {

	OCR* ocr = new Tesseract();
	ocr->init("/home/trainingFont/trainedModel", "eng");
	Overlap lap = Overlap();
	lap.Init("/home/localizationtests/volumen/overlap/C.png", ocr);
	lap.test();
	REQUIRE(lap.getPass()==true);
	lap.Init("/home/localizationtests/volumen/overlap/Multi_Correct.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == true);
	ocr->release();
	delete ocr;
}
TEST_CASE("OverlapNegative", "[Overlap]") {

	OCR* ocr = new Tesseract();
	ocr->init("/home/trainingFont/trainedModel", "eng");
	Overlap lap = Overlap();
	lap.Init("/home/localizationtests/volumen/overlap/F.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == false);
	lap.Init("/home/localizationtests/volumen/overlap/Multi_Fail.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == false);

	ocr->release();
	delete ocr;
}
TEST_CASE("Truncation", "[Truncation]") {

	std::string esperado = "Empezar jugar";
	Truncation t = Truncation(esperado);
	std::string entrada = "Empezar jugar"; // Varias truncadas
	t.test(entrada);
	REQUIRE(t.getPass() == true);

	esperado= "Hola este es un texto completo con varias palabras importantes";
	t = Truncation(esperado);
	entrada = "Hola este es un texto completo con varias palabras importantes";
	t.test(entrada);
	REQUIRE(t.getPass() == true);
	
}
TEST_CASE("TruncationNegative", "[Truncation]") {

	std::string esperado = "Hola este es un texto completo con varias palabras importantes";
	// Casos de prueba
	std::string entrada1 = "Hol est es un tex comple con var palab importantes"; // Varias truncadas
	std::string entrada3 = "Hola est es otro text"; // Diferente en algunas palabras
	std::string entrada4 = "Hola este es"; // Truncado desde "un"
	Truncation t = Truncation(esperado);
	t.test(entrada1);
	REQUIRE(t.getPass() == false);
	t.test(entrada3);
	REQUIRE(t.getPass() == false);
	t.test(entrada4);
	REQUIRE(t.getPass() == false);


}