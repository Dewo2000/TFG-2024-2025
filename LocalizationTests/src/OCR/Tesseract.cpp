#include "Tesseract.h"
#include <fstream> 
#include <string>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


bool Tesseract::init(std::string modelPath, std::string font)
{
    font.erase(std::remove_if(font.begin(), font.end(), ::isspace), font.end());
    _ocr = new tesseract::TessBaseAPI();
    	if (_ocr->Init(modelPath.c_str(), font.c_str())) {
    		std::cout << "fallo Init tess" << std::endl;
    		return false;
    }
        return true;
}

void Tesseract::release()
{
    if (_ocr != nullptr)
        _ocr->End();
}

bool Tesseract::getDirImgText(std::string imgPath, std::string outputPath)
{
    	//Creaci�n de carpetas de resultados
	std::string command = "sudo mkdir -p "+outputPath;
	std::system(command.c_str());
	command = "sudo chmod 777 "+outputPath;
	std::system(command.c_str());

    std::vector<std::string> pngFiles;
    DIR* dir;
    struct dirent* ent;

    // Abre el directorio
    if ((dir = opendir(imgPath.c_str())) != nullptr) {
        // Itera sobre las entradas del directorio
        while ((ent = readdir(dir)) != nullptr) {
            // Obtiene el nombre del archivo
            std::string filename = ent->d_name;
            // Verifica si la entrada es un archivo regular y tiene la extensi�n .png
            if (ent->d_type == DT_REG && filename.size() > 4 && filename.substr(filename.size() - 4) == ".png") {
                pngFiles.push_back(filename);
            }
        }
        // Cierra el directorio
        closedir(dir);
    }
    else {
        // Muestra un mensaje de error si el directorio no se puede abrir
        std::cerr << "No se pudo abrir el directorio: " << imgPath << std::endl;
    }
    //Procesamiento de cada imagen
    for (int i = 0; i < pngFiles.size(); i++) {
        imageInfo image;
        preprocessing(imgPath+"/" + pngFiles[i], image);
        _ocr->SetImage(image.data, image.cols, image.rows, 1, image.step);
        char* outText = _ocr->GetUTF8Text();
        std::string gtName;
        if (gtName != "") {
            std::string expected = readGT(gtName);
            std::vector<std::string> expectedLines = splitIntoLines(expected);
            std::vector<std::string> outputLines = splitIntoLines(outText);
            std::string cleanOutPut;
            for (const auto& expectedLine : expectedLines) {
                std::string bestMatch = findMostSimilarLine(expectedLine, outputLines, 0.8);
                if (!bestMatch.empty()) {
                    cleanOutPut += bestMatch + "\n";
                }
            }
            std::ofstream outFile(outputPath + "/" + std::to_string(i) + ".txt");
            if (outFile.is_open()) {
                outFile << cleanOutPut;
                outFile.close();
                std::cout << "Texto guardado" << std::endl;
            }
            else {
                std::cerr << "No se pudo abrir el archivo para escribir" << std::endl;
            }
        }
        else {
            std::ofstream outFile(outputPath + "/" + std::to_string(i) + ".txt");
            if (outFile.is_open()) {
                outFile << outText;
                outFile.close();
                std::cout << "Texto guardado" << std::endl;
            }
            else {
                std::cerr << "No se pudo abrir el archivo para escribir" << std::endl;
            }
        }
        	//Liberar recursos
        delete[] outText;
        delete image.data;
    }
    return false;
}
bool Tesseract::trainModel(std::string lan, std::string font, int iteration, bool clear)
{
    if (train(lan, font, iteration, clear)) {
        font.erase(std::remove_if(font.begin(), font.end(), ::isspace), font.end());
        std::string command = "sudo cp /home/tesseract_repos/tesstrain/data/" + font + "_data/" + font + "-" + lan + "-output/" + font + ".traineddata " + tessdatadir + "/" + font + ".traineddata ";
        std::cout << command << std::endl;
        system(command.c_str());
        return true;
    }
}
bool Tesseract::train(std::string lan, std::string font, int iteration ,bool clear)
{
    if (!generateGT(lan, font)) {
        std::cerr << "Error during GT\n";
        return false;
    }
    //Archivo log para la salida
    const char* output_file = "output.log";
    const char* error_file = "error.log";

    //Cadenas donde si aparecen entonces es un error
    std::string errorIndicators[] = {
        "There is no ground-truth",
        "Failed loading language",
        "Tesseract couldn't load any languages!"
    };
    //Eliminar espacios
    font.erase(std::remove_if(font.begin(), font.end(), ::isspace), font.end());

    std::string command = "sudo " + trainTess + " -l " + lan + " -f \"" + font + "\"" +" -it "+ std::to_string(iteration);
    if (clear)command = command + " -cl";
    //command = command + " > > (tee " + std::string(output_file) + ") 2> > (tee " + std::string(error_file) +" 2>&)";

    //command = command + " > " + std::string(output_file) + " 2> " + std::string(error_file);
    std::cout << command << std::endl;
    int res = system(command.c_str());

    // Leer la salida est�ndar
    std::ifstream output_stream(output_file);
    std::string output_message((std::istreambuf_iterator<char>(output_stream)), std::istreambuf_iterator<char>());
    output_stream.close();

    // Leer la salida de error
    std::ifstream error_stream(error_file);
    std::string error_message((std::istreambuf_iterator<char>(error_stream)), std::istreambuf_iterator<char>());
    error_stream.close();

    //Se elimina el log temporal y se escribe la salida
    std::remove(output_file);
    std::remove(error_file);
    /*std::cout << output_message << std::endl;
    std::cout << error_message << std::endl;*/

    //Error de fuente
    for (const auto& indicator : errorIndicators) {
        if (output_message.find(indicator) != std::string::npos || error_message.find(indicator) != std::string::npos) {
            return false;
        }
    }
    if (WIFEXITED(res) && WEXITSTATUS(res) == 0) {
        return true;
    }
    else {
        return false;
    }
}
bool Tesseract::generateGT(std::string lan, std::string font)
{
    //Archivo log para la salida
    const char* output_file = "output.log";
    const char* error_file = "error.log";
    
    //Cadenas donde si aparecen entonces es un error
    std::string errorIndicators[] = {
        "Please correct --font arg.",
    };

    //Comando del groundTruth con sudo , los argumentos lan y font , y se recoge la salida estandar de error
    std::string command = "sudo " + groundTruth + " -l " + lan + " -f \" " + font + "\"" + " > " + std::string(output_file) + " 2> " + std::string(error_file);
	int res = system(command.c_str());

    // Leer la salida est�ndar
    std::ifstream output_stream(output_file);
    std::string output_message((std::istreambuf_iterator<char>(output_stream)), std::istreambuf_iterator<char>());
    output_stream.close();

    // Leer la salida de error
    std::ifstream error_stream(error_file);
    std::string error_message((std::istreambuf_iterator<char>(error_stream)), std::istreambuf_iterator<char>());
    error_stream.close();

    //Se elimina el log temporal y se escribe la salida
    std::remove(output_file);
    std::remove(error_file);
    std::cout << output_message << std::endl;
    std::cout << error_message << std::endl;

    //Error de fuente
    for (const auto& indicator : errorIndicators) {
        if (output_message.find(indicator) != std::string::npos || error_message.find(indicator) != std::string::npos) {
            std::cerr << "Font error" << std::endl;
            return false;
        }
    }
    //Error del sistema o lenguaje no soportado por tesseract
    if (WIFEXITED(res) && WEXITSTATUS(res) == 0) {
        return true;
    }
    else {
        return false;
    }
   
}



