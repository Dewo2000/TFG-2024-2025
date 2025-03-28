#include "Truncation.h"
#include "Placeholders.h"
#include "OCR.h"
#include "Tesseract.h"
#include "Overlap.h"
#define CATCH_CONFIG_MAIN  // Esto define la funci�n main() para las pruebas
#include "catch2/catch.hpp"      // Incluir Catch2

TEST_CASE("Placeholders", "[Placeholders]") {

	
	std::string texto = "Texto con $*placeholder1* y tambi�n [otroPlaceholder] y {unoMas}. Texto con *placeholder1* y tambien [otroPlaceholder] y {unoMas}.";
	std::vector<std::pair<char, char>> delimitadores = {
		{'*', '*'},   // Placeholder entre *
		{'[', ']'},   // Placeholder entre []
		{'{', '}'},   // Placeholder entre {}
	};
	Placeholders h = Placeholders(delimitadores);
	h.test(texto);
	std::vector<PlaceholderResult> expect({ { 10,"*placeholder1*" },{75,"*placeholder1*"},
									{ 35,"[otroPlaceholder]" },
									 {100,"[otroPlaceholder]"},{ 55,"{unoMas}" },
									 {120,"{unoMas}"} });
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test("No");
	REQUIRE(h.getPass() == true);
}

TEST_CASE("PlaceholdersNegativo", "[Placeholders]") {


	std::string texto = "Texto con $*placeholder1* y tambi�n [otroPlaceholder] y {unoMas}. Texto con *placeholder1* y tambien [otroPlaceholder] y {unoMas}.";
	std::vector<std::pair<char, char>> delimitadores = {
		{'*', '*'},   // Placeholder entre *
		{'[', ']'},   // Placeholder entre []
		{'{', '}'},   // Placeholder entre {}
	};
	Placeholders h = Placeholders(delimitadores);
	h.test(texto);
	std::vector<PlaceholderResult> expect({ { 10,"*placeholder1*" },{75,"*placeholder1*"},
									{ 35,"[otroPlaceholder]" },
									 {100,"[otroPlaceholder]"},{ 55,"{unoMas}" },
									 {120,"{unoMas}"} });
	REQUIRE(h.getPass() == false);
	REQUIRE(h.getResult() == expect);
	h.test("No");
	REQUIRE(h.getPass() == true);
}

TEST_CASE("Overlap", "[Overlap]") {

	OCR* ocr = new Tesseract();
	ocr->init("/home/trainingFont/trainedModel", "eng");
	Overlap lap = Overlap();
	lap.Init("/home/localizationtests/volumen/overlap/C.png", ocr);
	lap.test();
	REQUIRE(lap.getPass()==true);
	lap.Init("/home/localizationtests/volumen/overlap/F.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == false);
	lap.Init("/home/localizationtests/volumen/overlap/Multi_Correct.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == true);
	lap.Init("/home/localizationtests/volumen/overlap/Multi_Fail.png", ocr);
	lap.test();
	REQUIRE(lap.getPass() == false);

	ocr->release();
	delete ocr;


}
TEST_CASE("Truncation", "[Truncation]") {

	std::string esperado = "Hola este es un texto completo con varias palabras importantes";

	// Casos de prueba
	std::string entrada1 = "Hol est es un tex comple con var palab import"; // Varias truncadas
	std::string entrada2 = "Hola este es un texto completo con varias palabras importantes"; // No truncado
	std::string entrada3 = "Hola est es otro text"; // Diferente en algunas palabras
	std::string entrada4 = "Hola este es"; // Truncado desde "un"
	Truncation t = Truncation(esperado);
	t.test(entrada1);
	REQUIRE(t.getPass() == false);
	t.test(entrada2);
	REQUIRE(t.getPass() == true);
	t.test(entrada3);
	REQUIRE(t.getPass() == false);
	t.test(entrada4);
	REQUIRE(t.getPass() == false);
	

}