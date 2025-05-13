#include "Similarity.h"
#include <iostream>
#include "Levenshtein.h"
using namespace std;

Similarity::Similarity(std::string expected)
{
	_expected = expected;
	_similarity = 0.0;
	_testPass = true;
}

Similarity::~Similarity()
{
}
void Similarity::test(const std::string& testString)
{
	int distance = Levenshtein::levenshteinDist(_expected, testString);
	int maxLen = std::max(_expected.size(), testString.size());
	if (maxLen == 0)_similarity = 100.0;
	else
	{
		_similarity = (1.0 - static_cast<double>(distance) / maxLen) * 100.0;
	}
}