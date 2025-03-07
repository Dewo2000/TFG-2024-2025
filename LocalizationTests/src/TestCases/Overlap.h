#pragma once
#ifndef OVERLAP_H
#define OVERLAP_H

#include "TestCase.h"
#include <vector>
struct ButtonLimits
{
	int x;
	int y;
	int w;
	int h;
};
class OCR;
struct BoundingBox;

class Overlap : TestCase {
public:
	Overlap();
	~Overlap();
	void test(const std::string& testString="") override;
	bool checkOverlap();
	bool Init(std::string imageUrl, OCR* ocr);
	bool getButtons();
	void setMinButtonSize(int w, int h);
private:
	std::vector<ButtonLimits> _buttons;
	std::vector<BoundingBox> _boxes;
	OCR* _ocr = nullptr;
	std::string _imageUrl;
	int _butMinW, _butMinH;
	bool _result;
};
#endif