#pragma once
#ifndef TRUNCATION_H
#define TRUNCATION_H

#include "TestCase.h"
#include <string>
#include <vector>

class Truncation : TestCase {
public:
	Truncation(std::string expected);
	~Truncation();
	void test(const std::string& testString) override;
	bool getPass() { return testPass; };
private:
    std::vector<std::string> split(const std::string& texto);
	std::string _expected;
	bool testPass;
};
#endif
