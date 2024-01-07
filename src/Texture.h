#pragma once
#include "pch.h"
#include "Utils.h"

class Texture
{
public:
   
    Texture();
    ~Texture();
    bool Load(const std::string& filePath);
    void createDefault();
    void Bind();
    PixelFormat format;
    Uint32 id;
    int width;
    int height;
};