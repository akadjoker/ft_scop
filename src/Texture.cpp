#include "Texture.h"    
#include "File.h"

//    #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
//       #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

Texture::Texture()

{
    id = 0;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
}

bool Texture::Load(const std::string &filePath)
{
      Uint8 *dst;
      int channels;

    File file(filePath, "rb");

    if (!file.IsOpen()) 
    {
        Log(2,"Error reading TGA data");    
        createDefault();
        return false;
    }
   
     Uint8 idLength = file.ReadUint8();
   
    file.ReadUint8();//skip

    Uint8 imageType = file.ReadUint8();
    if (imageType != 2)
    {
        Log(2,"Error reading TGA data, unsupported ");
        createDefault();
        return false;
    }

    file.Seek(9, true);

    width = file.ReadUint16();
    height = file.ReadUint16();
    Uint8  pixelDepth = file.ReadUint8();

    

    if (pixelDepth != 24 && pixelDepth != 32)
    {
        createDefault();
        Log(2,"Error reading TGA data, unsupported pixel depth");
        return false;
    }

    file.Seek(idLength +1, true);
    channels = (pixelDepth / 8);

    Log(1,"Image  (%d %d)   %d  %d",width,height,pixelDepth,channels);


    Uint32 imageSize = width * height * (pixelDepth / 8);
    dst = new Uint8[imageSize];
    file.Read(dst, imageSize);




    if (pixelDepth==24)
    {

        //flip vertical 

        for (int y = 0; y < height / 2; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index1 = (y * width + x) * 3;
                int index2 = ((height - 1 - y) * width + x) * 3;

                Uint8 temp = dst[index1];
                dst[index1] = dst[index2];
                dst[index2] = temp;

                temp = dst[index1 + 1];
                dst[index1 + 1] = dst[index2 + 1];
                dst[index2 + 1] = temp;

                temp = dst[index1 + 2];
                dst[index1 + 2] = dst[index2 + 2];
                dst[index2 + 2] = temp;
            }
        }

        for (Uint32 i = 0; i < imageSize; i += 3) //swap R and B
        {
            Uint8 temp = dst[i];
            dst[i] = dst[i + 2];
            dst[i + 2] = temp;
        }


        format = PixelFormat::R8G8B8;
    }
    else
    {
        
        //flip vertical

        for (int y = 0; y < height / 2; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index1 = (y * width + x) * 4;
                int index2 = ((height - 1 - y) * width + x) * 4;

                Uint8 temp = dst[index1];
                dst[index1] = dst[index2];
                dst[index2] = temp;

                temp = dst[index1 + 1];
                dst[index1 + 1] = dst[index2 + 1];
                dst[index2 + 1] = temp;

                temp = dst[index1 + 2];
                dst[index1 + 2] = dst[index2 + 2];
                dst[index2 + 2] = temp;

                temp = dst[index1 + 3];
                dst[index1 + 3] = dst[index2 + 3];
                dst[index2 + 3] = temp;
            }
        }

        for (Uint32 i = 0; i < imageSize; i += 4) //swap R and B
        {
            Uint8 temp = dst[i];
            dst[i] = dst[i + 2];
            dst[i + 2] = temp;
        }


        format = PixelFormat::R8G8B8A8;
    }
  
    // dst=  stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    // if (dst == nullptr)
    // {
    //     Log(2,"Error reading TGA data, unsupported pixel depth");
    //     return false;
    // }

    if (channels==3)
    {
        format = PixelFormat::R8G8B8;
    }
    else
    {
        format = PixelFormat::R8G8B8A8;
    }

     Log(0,"Image  %d %d %d ",width,height,channels);

    id = CreateTexture(dst, width, height,  format);
  //  stbi_write_png("test.png",width,height,channels,dst, width*channels);

    delete[] dst;
    // Everything went well :)
    
    return true;

}

void Texture::createDefault()
{
    width = 1;
    height = 1;
    Uint8 pixels[4] = { 255, 255, 255, 255 };
    format = PixelFormat::R8G8B8A8;
    id = CreateTexture(pixels, width, height, format);
}

void Texture::Bind()
{
    if (id != 0)
        glBindTexture(GL_TEXTURE_2D, id);
}
