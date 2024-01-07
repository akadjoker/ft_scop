#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>
#include <iostream>


#include "image_data.h" // Inclua o arquivo de dados da imagem gerado

int main() 
{

    if (stbi_write_png("output_image.png", imageWidth, imageHeight, 4, imageData, 0) != 0) 
    {
        std::cout << "Imagem escrita com sucesso." << std::endl;
    } else {
        std::cerr << "Erro ao escrever a imagem." << std::endl;
        return 1;
    }

    return 0;
}

