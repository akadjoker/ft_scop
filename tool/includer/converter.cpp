#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    const char* filePath = "font2.png"; // Substitua com o caminho da sua imagem

    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);

    if (!imageData) {
        std::cerr << "Erro ao carregar a imagem." << std::endl;
        return 1;
    }

    std::ofstream outputFile("image_data.h");
    if (!outputFile.is_open()) {
        std::cerr << "Erro ao criar o arquivo de inclusão." << std::endl;
        return 1;
    }

    outputFile << "#pragma once\n\n";
    outputFile << "constexpr int imageWidth = " << width << ";\n";
    outputFile << "constexpr int imageHeight = " << height << ";\n";
    outputFile << "constexpr unsigned char imageData[] = {";

    for (int i = 0; i < width * height * channels; ++i) {
        if (i % 20 == 0) outputFile << "\n    ";
        outputFile << static_cast<int>(imageData[i]);
        if (i != width * height * channels - 1) outputFile << ",";
    }

    outputFile << "\n};\n";
    outputFile.close();

    stbi_image_free(imageData);

    std::cout << "Arquivo de inclusão gerado com sucesso." << std::endl;
    return 0;
}

