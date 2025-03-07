#include "Truncation.h"
#include "Placeholders.h"
#define CATCH_CONFIG_MAIN  // Esto define la función main() para las pruebas
#include "catch2/catch.hpp"      // Incluir Catch2

TEST_CASE("Placeholders", "[Placeholders]") {

	
	std::string texto = "Texto con *placeholder1* y también [otroPlaceholder] y {unoMas}. Texto con *placeholder1* y tambien [otroPlaceholder] y {unoMas}.";
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

	REQUIRE(h.getResult() == expect);
}
TEST_CASE("Overlap", "[Overlap]") {



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
	//t.test(entrada3);
	//REQUIRE(t.getPass() == false);
	//t.test(entrada4);
	//REQUIRE(t.getPass() == false);
	//t.test(entrada2);
	//REQUIRE(t.getPass() == true);

}