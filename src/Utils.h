#pragma once
#include "pch.h"


#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"
#define CONSOLE_COLOR_CYAN "\033[0;36m"


enum PixelFormat
{
    GRAYSCALE = 1,     
    GRAY_ALPHA,        
    R8G8B8,            
    R8G8B8A8,          
};


void Log(int severity, const char* fmt, ...);
char *LoadFileText(const char *fileName);

bool FileExists(const std::string& filename) ;

Uint32 LoadShaderProgram(Uint32 vShaderId, Uint32 fShaderId);
Uint32 CompileShader(const char *shaderCode, int type);

Uint32 CreateTexture(const void *data, int width, int height, PixelFormat format);

Uint32 LoadShaderFromFile(const char *vsFileName, const char *fsFileName);
Uint32 LoadShaderFromMemory(const char *vsCode, const char *fsCode);

void UnloadVertexArray(Uint32 vaoId);

void UnloadVertexBuffer(Uint32 vboId);

void UnloadTexture(Uint32 id);

Uint32 GetTicks(void);
double GetTime(void);

std::string GetFileExtension(const std::string& filename) ;
std::string GetFileName(const std::string& filepath) ;
std::string GetFileNameNoExtension(const std::string& filepath) ;
std::string GetFilePath(const std::string& filepath) ;
std::string ToLower(const std::string& str) ;

std::string StringReplace(const std::string& haystack, const std::string& needle, const std::string& replacement);
std::vector<std::string> Split(const std::string &s, char delim);
