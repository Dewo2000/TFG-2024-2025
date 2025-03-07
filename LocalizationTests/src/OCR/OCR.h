#pragma once
#ifndef OCR_H
#define OCR_H
#include <iostream>
#include <cstdlib>
#include <vector>
namespace cv {
	class Mat;
}
struct imageInfo {
	unsigned char* data;
	int cols;
	int rows;
	int step;
};
struct BoundingBox {
	int x;
	int y;
	int x2;
	int y2;
};
class OCR
{
public:
	virtual ~OCR() {};
	virtual bool init(std::string modelPath,std::string font) = 0;
	virtual void release() = 0;
	virtual bool getDirImgText(std::string imgPath, std::string outputPath)=0;
	virtual bool trainModel(std::string lan, std::string font, int iteration, bool clear = false)=0;
	virtual std::vector<BoundingBox> getBoundingBoxes(std::string imageUrl) {};
protected:
	std::string readGT(const std::string& rutaArchivo);
	std::string findMostSimilarLine(const std::string& target, const std::vector<std::string>& recognizedLines, double threshold);
	std::vector<std::string> splitIntoLines(const std::string& text);
	std::string _volumen = "/home/localizationtests/volumen/";
	bool preprocessing(std::string iName, imageInfo& image);
	
};


#endif