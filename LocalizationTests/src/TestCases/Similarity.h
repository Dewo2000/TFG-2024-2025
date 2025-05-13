#pragma once
#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "TestCase.h"
#include <string>
#include <vector>

class Similarity : TestCase {
public:
	Similarity(std::string expected);
	~Similarity();
	void test(const std::string& testString) override;
	bool getPass() override { return _testPass; };
	double getSimilarity() { return _similarity; };
private:
	std::vector<std::string> split(const std::string& texto);
	std::string _expected;
	bool _testPass;
	double _similarity;
};
#endif