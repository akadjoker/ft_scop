#pragma once
#include "pch.h"
#include "Math.h"

class Shader;
class Texture;

#define LINES                                0x0001     
#define TRIANGLES                            0x0004      
#define QUADS                                0x0008  
#define BATCH_DRAWCALLS                      128


struct VertexBatch
{
    VertexBatch()
    {
        position.x = 0;
        position.y = 0;
        position.z = 0;
        texcoord.x = 0;
        texcoord.y = 0;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
    }
    VertexBatch(float x, float y, float z, float u, float v, float r, float g, float b, float a)
    {
        position.x = x;
        position.y = y;
        position.z = z;
        texcoord.x = u;
        texcoord.y = v;
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }


    Vector3 position;
    Vector2 texcoord;
    Color color;
};


struct RenderBuffer 
{
    int elementCount;           
    std::vector<VertexBatch>  vertices;            
    std::vector<unsigned int>  indices;    
    unsigned int vaoId;        
    unsigned int vboId;     
    unsigned int eboId;
} ;

struct DrawCall 
{
    int mode;                 
    int vertexCount;          
    int vertexAlignment;      
    unsigned int textureId;   
};


struct RenderBatch 
{
 
    RenderBatch();
    ~RenderBatch();

    void Init(int numBuffers, int bufferElements);
    void Release();


    void Render();

    void Mode(int mode);       


      void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, const Color &color);
      void DrawCircle(int centerX, int centerY, float radius, const Color &color, bool fill = false);

      void DrawRectangle(int posX, int posY, int width, int height, const Color &color,bool fill = false);
      void DrawRectangle(const Rectangle &rect, const Color &color, bool fill = false);
   
      
      
      void DrawLine3D(const Vector3 &start, const Vector3 &end, const Color &color);
      void DrawLine3D(float x1, float y1, float z1, float x2, float y2, float z2, const Color &color);
      void DrawGrid(int slices, float spacing);

   

    void DrawTexture(Texture *texture, float x, float y,float width, float height, const Rectangle &clip, const Color &color);
    void DrawTexture(Texture *texture, float x, float y, const Rectangle &clip, const Color &color);
    void DrawTexture(Texture *texture, float x, float y, float width, float height,  const Color &color);

    
    void Vertex2f(float x, float y);          
    void Vertex3f(float x, float y, float z);     
    void TexCoord2f(float x, float y);          

    void Color4f(float r, float g, float b, float a); 
    void Color3f(float r, float g, float b); 

    void SetMatrix(const Matrix &m);



    private:
        bool CheckLimits(int vCount);
        void SetTexture(unsigned int id);

    int bufferCount;            
    int currentBuffer;          
    int drawCounter;            
    float currentDepth;         
    int vertexCounter;
    unsigned int defaultTextureId;  
    unsigned int defaultShaderId;

    unsigned int mpvId;
    unsigned int textId;

    std::vector<DrawCall*> draws;
    std::vector<RenderBuffer*> vertexBuffer;

    float texcoordx, texcoordy;         
    float colorr, colorg, colorb, colora;
    Matrix matrix;

    


    

};