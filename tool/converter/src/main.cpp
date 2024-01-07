#include <algorithm> 
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::string GetFileExtension(const std::string& filename) 
{
    size_t dotPosition = filename.find_last_of(".");
    if (dotPosition != std::string::npos) 
    {
        return filename.substr(dotPosition + 1);
    }
    return ""; 
}

std::string GetFileName(const std::string& filepath) 
{
    size_t slashPosition = filepath.find_last_of("/\\");
    if (slashPosition != std::string::npos) {
        return filepath.substr(slashPosition + 1);
    }
    return filepath;
}

std::string GetFileNameNoExtension(const std::string& filepath) 
{
    std::string filename = GetFileName(filepath);
    size_t dotPosition = filename.find_last_of(".");
    if (dotPosition != std::string::npos) 
    {
        return filename.substr(0, dotPosition);
    }
    return filename;

}

std::string GetFilePath(const std::string& filepath) 
{
    size_t slashPosition = filepath.find_last_of("/\\");
    if (slashPosition != std::string::npos) 
    {
        return filepath.substr(0, slashPosition + 1);
    }
    return "";
    
}

std::string ToLower(const std::string& str) 
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}




int main()
{



    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        return -1;
    }


    SDL_Window* window = SDL_CreateWindow("scop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 440,100,   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) 
    {

        SDL_Quit();
        return -1;
    }
   
    bool quit = false;
    while (!quit) 
    {
      
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {

                case SDL_QUIT:
                {
                     quit = true;
                    break;    
                }
                case SDL_DROPFILE:          
                {
                    char *dropped_filedir = event.drop.file;
                

                    std::string extension = ToLower(GetFileExtension(dropped_filedir));
                    std::string filename = GetFileNameNoExtension(dropped_filedir);
                    std::string path = GetFilePath(dropped_filedir);

                    SDL_Log("File extension: %s", extension.c_str());
                    SDL_Log("File name: %s", filename.c_str());
                    SDL_Log("File path: %s", path.c_str());

                    int width, height, nrChannels;

                    unsigned char* data = stbi_load(dropped_filedir, &width, &height, &nrChannels, 0);
                    if (data) 
                    {
                        SDL_Log("Image loaded: %dx%d %d", width, height, nrChannels);
                        std::string outputFilename = path + filename + ".tga";
                        SDL_Log("Saving to: %s", outputFilename.c_str());
                        stbi_write_tga(outputFilename.c_str(), width, height, nrChannels, data);
                        stbi_image_free(data);
                    }
                    else 
                    {
                        SDL_Log("Failed to load image");
                    }

                   


       
                    break;
                }
                  case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)      quit = true;
                    
                    break;  
                }

               

        }
        }

      



    }
  
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}

