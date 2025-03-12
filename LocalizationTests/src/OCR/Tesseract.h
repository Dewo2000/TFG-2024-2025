#pragma once
#ifndef TESSERACT_H
#define TESSERACT_H
#include "OCR.h"
namespace tesseract {
	class TessBaseAPI;
}
class Tesseract : public OCR
{
public:
	Tesseract() {};
	~Tesseract() {};
	bool init(std::string modelPath, std::string font)override;
	void release() override;
	bool getDirImgText(std::string imgPath, std::string outputPath) override;
	bool trainModel(std::string lan, std::string font, int iteration,bool clear=false) override;
	std::vector<LBox> getBoundingBoxes(std::string imageUrl)override;
private:
	const std::string trainFolder = "/home/trainingFont";
	const std::string groundTruth = "python3 " + trainFolder + "/ground_truth_exec.py";
	const std::string trainTess = "python3 " + trainFolder + "/trainTess_exec.py";
	const std::string tessdatadir = trainFolder + "/trainedModel";
	bool generateGT(std::string lan, std::string font);
	bool train(std::string lan, std::string font, int iteration, bool clear = false);
	tesseract::TessBaseAPI* _ocr = nullptr;
};

#endif