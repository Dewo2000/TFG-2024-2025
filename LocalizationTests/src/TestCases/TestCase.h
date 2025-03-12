#pragma once
#ifndef TESTCASE_H
#define TESTCASE_H

#include <string>
#include <spdlog/spdlog.h>
class TestCase {
public:
	virtual void test(const std::string& testString) = 0;
	virtual bool getPass() {};
};
#endif
