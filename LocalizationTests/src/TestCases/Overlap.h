#pragma once
#ifndef OVERLAP_H
#define OVERLAP_H

#include "TestCase.h"
#include <vector>
class OCR;
struct LBox;
struct ButtonLimits;
class Overlap : TestCase {
public:
	Overlap();
	~Overlap();
	void test(const std::string& testString="") override;
	bool checkOverlap();
	bool Init(std::string imageUrl, OCR* ocr);
	void setMinButtonSize(int w, int h);
	bool getPass() override { return _testPass; };
private:
	std::vector<ButtonLimits> _buttons;
	std::vector<LBox> _boxes;
	OCR* _ocr = nullptr;
	std::string _imageUrl;
	int _butMinW, _butMinH;
	bool _testPass;
};
#endif