#pragma once
#include "pch.h"
#include "Utils.h"


class Texture
{
public:
   
    Texture();
    ~Texture();
    bool Load(const std::string& filePath,bool flip=false);
    void createDefault();
    void Bind(int layer=0);
    void Release();
    PixelFormat format;
    Uint32 id;
    int width;
    int height;
};