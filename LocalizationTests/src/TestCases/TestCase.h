#pragma once
#ifndef TESTCASE_H
#define TESTCASE_H

#include <string>

class TestCase {
public:
	virtual void test(const std::string& testString) = 0;
};
#endif
