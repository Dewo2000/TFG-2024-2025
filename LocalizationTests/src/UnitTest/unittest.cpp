
#include "Truncation.h"
#define CATCH_CONFIG_MAIN  // Esto define la función main() para las pruebas
#include "catch2/catch.hpp"      // Incluir Catch2

int add(int a, int b) {
	return a + b;
}



TEST_CASE("Addition works", "[add]") {
	REQUIRE(add(2, 3) == 5);
	REQUIRE(add(-2, -3) == -5);
	REQUIRE(add(-2, 3) == 1);
}
TEST_CASE("Placeholders", "[]") {

	std::string esperado = "Hola este es un texto completo con varias palabras importantes";

	// Casos de prueba
	std::string entrada1 = "Hol est es un tex comple con var palab import"; // Varias truncadas
	std::string entrada2 = "Hola este es un texto completo con varias palabras importantes"; // No truncado
	std::string entrada3 = "Hola est es otro text"; // Diferente en algunas palabras
	std::string entrada4 = "Hola este es"; // Truncado desde "un"
	Truncation t = Truncation(esperado);
	t.test(entrada1);
////t.test(entrada3);
////t.test(entrada4);
	REQUIRE(t.getPass() == false);
	t.test(entrada2);
	REQUIRE(t.getPass() == true);
}