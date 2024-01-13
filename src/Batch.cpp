#include "Mesh.h"
#include "Texture.h"    
#include "Utils.h"
#include "Shader.h"
#include "Batch.h"


const Color Color::WHITE(1.0f,1.0f,1.0f);
const Color Color::GRAY(0.5f,0.5f,0.5f);
const Color Color::BLACK(0.0f,0.0f,0.0f);
const Color Color::RED(1.0f,0.0f,0.0f);
const Color Color::GREEN(0.0f,1.0f,0.0f);
const Color Color::BLUE(0.0f,0.0f,1.0f);
const Color Color::CYAN(0.0f,1.0f,1.0f);
const Color Color::MAGENTA(1.0f,0.0f,1.0f);
const Color Color::YELLOW(1.0f,1.0f,0.0f);
const Color Color::ORANGE(1.0f,0.5f,0.0f);
const Color Color::PURPLE(0.5f,0.0f,0.5f);
const Color Color::PINK(1.0f,0.0f,0.5f);
const Color Color::BROWN(0.5f,0.25f,0.0f);
const Color Color::CORNFLOWER(0.4f,0.6f,0.9f);
const Color Color::BABYBLUE(0.6f,0.8f,1.0f);
const Color Color::LIME(0.5f,1.0f,0.0f);
const Color Color::PEACH(1.0f,0.8f,0.6f);
const Color Color::LAVENDER(0.8f,0.6f,1.0f);
const Color Color::TURQUOISE(0.0f,1.0f,1.0f);
const Color Color::GOLD(1.0f,0.8f,0.0f);
const Color Color::SILVER(0.75f,0.75f,0.75f);
const Color Color::BRONZE(0.8f,0.5f,0.2f);
const Color Color::SKYBLUE(0.4f,0.8f,1.0f);
const Color Color::ShadesOfGray(0.5f,0.5f,0.5f);


RenderBatch::RenderBatch()
{
}

RenderBatch::~RenderBatch()
{
     Release();
}

void RenderBatch::Init(int numBuffers, int bufferElements)
{


    const char *defaultVShaderCode =
    "#version 320   es                    \n"
     "precision mediump float;           \n"
    "in vec3 vertexPosition;            \n"
    "in vec2 vertexTexCoord;            \n"
    "in vec4 vertexColor;               \n"
    "out vec2 fragTexCoord;             \n"
    "out vec4 fragColor;                \n"
    "uniform mat4 mvp;                  \n"
    "void main()                        \n"
    "{                                  \n"
    "    fragTexCoord = vertexTexCoord; \n"
    "    fragColor = vertexColor;       \n"
    "    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
    "}                                  \n";


    const char *defaultFShaderCode =
    "#version 320  es    \n"
    "precision mediump float;           \n"
    "in vec2 fragTexCoord;              \n"
    "in vec4 fragColor;                 \n"
    "out vec4 finalColor;               \n"
    "uniform sampler2D texture0;        \n"
    "void main()                        \n"
    "{                                  \n"
    "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
    "    finalColor = texelColor*fragColor;        \n"
    "}                                  \n";

    unsigned int vShaderId = 0;
    unsigned int fShaderId = 0;

    vShaderId = CompileShader(defaultVShaderCode, GL_VERTEX_SHADER);
    fShaderId = CompileShader(defaultFShaderCode, GL_FRAGMENT_SHADER);
    if (vShaderId != 0 && fShaderId != 0)
    {
        defaultShaderId = LoadShaderProgram(vShaderId, fShaderId);
        if (defaultShaderId != 0)
        {
            Log(0, "SHADER: [ID %i] Default shader loaded successfully", defaultShaderId);

             glDeleteShader(vShaderId);
             glDeleteShader(fShaderId);
        }
    } else 
    {
        Log(2,"SHADER: Failed to load default shader");
        return ;
    }

    mpvId = glGetUniformLocation(defaultShaderId, "mvp");
    textId = glGetUniformLocation(defaultShaderId, "texture0");

    glUniform1i(textId, 0);

    unsigned char pixels[4] = { 255, 255, 255, 255 };  
    defaultTextureId = CreateTexture(pixels, 1, 1, PixelFormat::R8G8B8A8);

    if (defaultTextureId != 0) Log(0, "TEXTURE: [ID %i] Default texture loaded successfully", defaultTextureId);



    for (int i=0;i<numBuffers;i++)
    {
        vertexBuffer.push_back(new RenderBuffer());
    }

      vertexCounter = 0;
      currentBuffer = 0;

        colora = 1.0f;
        colorr = 1.0f;
        colorg = 1.0f;
        colorb = 1.0f;
    

    for (int i = 0; i < numBuffers; i++)
    {
        vertexBuffer[i]->elementCount = bufferElements;

        int k = 0;


        for (int j = 0; j <= bufferElements; j ++)
        {
            
            vertexBuffer[i]->vertices.push_back(VertexBatch());
            vertexBuffer[i]->vertices.push_back(VertexBatch());
            vertexBuffer[i]->vertices.push_back(VertexBatch());
            vertexBuffer[i]->vertices.push_back(VertexBatch());
          

            


            vertexBuffer[i]->indices.push_back(k);
            vertexBuffer[i]->indices.push_back(k + 1);
            vertexBuffer[i]->indices.push_back(k + 2);
            vertexBuffer[i]->indices.push_back(k);
            vertexBuffer[i]->indices.push_back(k + 2);
            vertexBuffer[i]->indices.push_back(k + 3);
            

            k+=4;
        }

      
    }

  
    for (int i = 0; i <numBuffers; i++)
    {
        glGenVertexArrays(1,&vertexBuffer[i]->vaoId);
        glBindVertexArray(vertexBuffer[i]->vaoId);

        glGenBuffers(1, &vertexBuffer[i]->vboId);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]->vboId);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer[i]->vertices.size()*sizeof(VertexBatch), vertexBuffer[i]->vertices.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBatch), reinterpret_cast<void*>(offsetof(VertexBatch, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBatch), reinterpret_cast<void*>(offsetof(VertexBatch, texcoord)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBatch), reinterpret_cast<void*>(offsetof(VertexBatch, color)));



        glGenBuffers(1, &vertexBuffer[i]->eboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer[i]->eboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  vertexBuffer[i]->indices.size() * sizeof(unsigned int), vertexBuffer[i]->indices.data(), GL_STATIC_DRAW);
    }


    
     glBindVertexArray(0);
    
    



    for (int i = 0; i < BATCH_DRAWCALLS; i++)
    {
        draws.push_back(new DrawCall());
        draws[i]->mode = QUADS;
        draws[i]->vertexCount = 0;
        draws[i]->vertexAlignment = 0;
        draws[i]->textureId =defaultTextureId;
    }

    bufferCount = numBuffers;   
    drawCounter = 1;            
    currentDepth = -1.0f;       
}

void RenderBatch::Release()
{
    if (vertexBuffer.size() == 0) return;
    for (int i = 0; i < (int)vertexBuffer.size(); i++)
    {
        UnloadVertexBuffer(vertexBuffer[i]->vboId);
        UnloadVertexBuffer(vertexBuffer[i]->eboId);
        UnloadVertexArray(vertexBuffer[i]->vaoId);
    }
    for (int i = 0; i < (int)draws.size(); i++)
    {
        delete draws[i];
    }
    for (int i = 0; i < (int)vertexBuffer.size(); i++)
    {
        vertexBuffer[i]->vertices.clear();
        vertexBuffer[i]->indices.clear();
        delete vertexBuffer[i];
    }
    vertexBuffer.clear();
    UnloadTexture(defaultTextureId);
    glDeleteProgram(defaultShaderId);    
}

void RenderBatch::Render()
{
        if (vertexCounter > 0)
        {
  
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[currentBuffer]->vboId);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCounter * sizeof(VertexBatch), vertexBuffer[currentBuffer]->vertices.data());

      
            
            glUseProgram(defaultShaderId);
            GLfloat mat[16]=
            {
                    matrix.m0, matrix.m1, matrix.m2, matrix.m3,
                    matrix.m4, matrix.m5, matrix.m6, matrix.m7,
                    matrix.m8, matrix.m9, matrix.m10, matrix.m11,
                    matrix.m12, matrix.m13, matrix.m14, matrix.m15
            };


            glUniformMatrix4fv(mpvId, 1, false, mat);
            glUniform1i(textId, 0);
            glBindVertexArray(vertexBuffer[currentBuffer]->vaoId);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer[currentBuffer]->eboId);
            glActiveTexture(GL_TEXTURE0);

         //   Log(0,"draw counter %d vertex %d %d ",drawCounter,vertexCounter, draws[0]->vertexCount/4*6);   

            for (int i = 0, vertexOffset = 0; i < drawCounter; i++)
            {

                glBindTexture(GL_TEXTURE_2D, draws[i]->textureId);

                int mode = draws[i]->mode;
                

               if ((mode == LINES) || (mode == TRIANGLES)) 
                    glDrawArrays(mode, vertexOffset, draws[i]->vertexCount);
               else
                    glDrawElements(GL_TRIANGLES, draws[i]->vertexCount/4*6, GL_UNSIGNED_INT,(GLvoid *)(vertexOffset/4*6*sizeof(unsigned int)));
               

               vertexOffset += (draws[i]->vertexCount + draws[i]->vertexAlignment);
            }

           
        }
    glBindTexture(GL_TEXTURE_2D, 0);    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    glUseProgram(0);    
    vertexCounter = 0;
    currentDepth = -1.0f;
    for (int i = 0; i < BATCH_DRAWCALLS; i++)
    {
        draws[i]->mode =QUADS;
        draws[i]->vertexCount = 0;
        draws[i]->textureId = defaultTextureId;
    }
    drawCounter = 1;
    currentBuffer++;
    if (currentBuffer >= bufferCount) currentBuffer = 0;
}

void RenderBatch::Mode(int mode)
{
      if (draws[drawCounter - 1]->mode != mode)
    {
        if (draws[drawCounter - 1]->vertexCount > 0)
        {
            if (draws[drawCounter - 1]->mode == LINES) 
            {
                draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4) ? draws[drawCounter - 1]->vertexCount : draws[drawCounter - 1]->vertexCount%4);
            }
            else 
            if (draws[drawCounter - 1]->mode == TRIANGLES) 
            {
                draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4)? 1 : (4 - (draws[drawCounter - 1]->vertexCount%4)));
            }
            else draws[drawCounter - 1]->vertexAlignment = 0;

            if (!CheckLimits(draws[drawCounter - 1]->vertexAlignment))
            {
                vertexCounter += draws[drawCounter - 1]->vertexAlignment;
                drawCounter++;
            }
        }

        if (drawCounter >= BATCH_DRAWCALLS) Render();

        draws[drawCounter - 1]->mode = mode;
        draws[drawCounter - 1]->vertexCount = 0;
        draws[drawCounter - 1]->textureId = defaultTextureId;
    }
}


void RenderBatch::Vertex2f(float x, float y)
{
    Vertex3f(x,y,currentDepth);
}

void RenderBatch::Vertex3f(float tx, float ty, float tz)
{
     if (vertexCounter > (vertexBuffer[currentBuffer]->elementCount*4 - 4))
    {
        if ((draws[drawCounter - 1]->mode == LINES) &&  (draws[drawCounter - 1]->vertexCount%2 == 0))
        {
            CheckLimits(2 + 1);
        }
        else if ((draws[drawCounter - 1]->mode == TRIANGLES) &&     (draws[drawCounter - 1]->vertexCount%3 == 0))
        {
            CheckLimits(3 + 1);
        }
        else if ((draws[drawCounter - 1]->mode == QUADS) && (draws[drawCounter - 1]->vertexCount%4 == 0))
        {
            CheckLimits(4 + 1);
        }
    }

    vertexBuffer[currentBuffer]->vertices[vertexCounter].position.set(tx,ty,tz);
    vertexBuffer[currentBuffer]->vertices[vertexCounter].texcoord.set(texcoordx,texcoordy);
    vertexBuffer[currentBuffer]->vertices[vertexCounter].color.set(colorr,colorg,colorb,colora);

    


    vertexCounter++;
    draws[drawCounter - 1]->vertexCount++;
}

void RenderBatch::TexCoord2f(float x, float y)
{
    texcoordx = x;
    texcoordy = y;
}

void RenderBatch::Color4f(float r, float g, float b, float a)
{
    colorr = r;
    colorg = g;
    colorb = b;
    colora = a;
}

void RenderBatch::Color3f(float r, float g, float b)
{
    colorr = r;
    colorg = g;
    colorb = b;
    colora = 1.0f;
}

void RenderBatch::SetMatrix(const Matrix &m)
{
    this->matrix = m;
}

bool RenderBatch::CheckLimits(int vCount)
{
    
    bool overflow = false;


    if ((vertexCounter + vCount) >=    (vertexBuffer[currentBuffer]->elementCount*4))
    {
        overflow = true;
        int currentMode = draws[drawCounter - 1]->mode;
        int currentTexture = draws[drawCounter - 1]->textureId;

        Render();
        draws[drawCounter - 1]->mode = currentMode;
        draws[drawCounter - 1]->textureId = currentTexture;
    }


    return overflow;
}

void RenderBatch::SetTexture(unsigned int id)
{
    if (id == 0)
    {
        if (vertexCounter >=    vertexBuffer[currentBuffer]->elementCount*4)
        {
            Render();
        }
    }
    else
    {
        if (draws[drawCounter - 1]->textureId != id)
        {
            if (draws[drawCounter - 1]->vertexCount > 0)
            {
                if (draws[drawCounter - 1]->mode == LINES) draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4)? draws[drawCounter - 1]->vertexCount : draws[drawCounter - 1]->vertexCount%4);
                else if (draws[drawCounter - 1]->mode == TRIANGLES) draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4)? 1 : (4 - (draws[drawCounter - 1]->vertexCount%4)));
                else draws[drawCounter - 1]->vertexAlignment = 0;

                if (!CheckLimits(draws[drawCounter - 1]->vertexAlignment))
                {
                    vertexCounter += draws[drawCounter - 1]->vertexAlignment;
                    drawCounter++;
                }
            }

            if (drawCounter >= BATCH_DRAWCALLS) Render();

            draws[drawCounter - 1]->textureId = id;
            draws[drawCounter - 1]->vertexCount = 0;
        }

    }
}


void RenderBatch::DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, const Color &color)
{
    Mode(LINES);
    Color4f(color.r, color.g, color.b, color.a);
    Vertex2f(startPosX, startPosY);
    Vertex2f(endPosX, endPosY);
}

void RenderBatch::DrawCircle(int centerX, int centerY, float radius, const Color &color, bool fill)
{
        SetTexture(0);
    if (fill)
    {
        Mode(TRIANGLES);
        Color4f(color.r, color.g, color.b, color.a);
        float x = centerX;
        float y = centerY;
        float angle = 0.0f;
        float angleInc = 1.0f / radius;
        for (int i = 0; i < 360; i++)
        {
            Vertex2f(x, y);
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
            angle += angleInc;
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
        }
    }
    else
    {
        Mode(LINES);
        Color4f(color.r, color.g, color.b, color.a);
        float x = centerX;
        float y = centerY;
        float angle = 0.0f;
        float angleInc = 1.0f / radius;
        for (int i = 0; i < 360; i++)
        {
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
            angle += angleInc;
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
        }
    }
}

void RenderBatch::DrawRectangle(int posX, int posY, int width, int height, const Color &color, bool fill)
{
    SetTexture(0);
    if (fill)
    {
        Mode(TRIANGLES);
     

        float x = posX;
        float y = posY;

        Color4f(color.r, color.g, color.b, color.a);
        

        Vertex2f(x   , y);
        Vertex2f(x, y + height);
        Vertex2f( x + width  , y);

        Vertex2f( x + width   , y);
        Vertex2f(x , y + height);
        Vertex2f( x + width, y + height);
    
    } else 
    {
        Mode(LINES);
        Color4f(color.r, color.g, color.b, color.a);

        Vertex2f(posX , posY );
        Vertex2f(posX + width, posY );

        Vertex2f(posX + width, posY );
        Vertex2f(posX + width, posY + height);

        Vertex2f(posX + width, posY + height);
        Vertex2f(posX , posY + height);

        Vertex2f(posX , posY + height);
        Vertex2f(posX , posY );
    
    }
}

void RenderBatch::DrawRectangle(const Rectangle &rect, const Color &color, bool fill)
{
    DrawRectangle((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, color, fill);
}

void RenderBatch::DrawLine3D(const Vector3 &start, const Vector3 &end, const Color &color)
{
    DrawLine3D(start.x, start.y, start.z, end.x, end.y, end.z, color);
}

void RenderBatch::DrawLine3D(float x1, float y1, float z1, float x2, float y2, float z2, const Color &color)
{
    Mode(LINES);
    Color4f(color.r, color.g, color.b, color.a);
    Vertex3f(x1, y1, z1);
    Vertex3f(x2, y2, z2);
}

void RenderBatch::DrawGrid(int slices, float spacing)
{

    DrawLine3D(0.0f,0.5f,0.0f,1,0.5f,0,Color(1,0,0,1));
    DrawLine3D(0.0f,0.5f,0.0f,0,1.5f,0,Color(0,1,0,1));
    DrawLine3D(0.0f,0.5f,0.0f,0,0.5f,1,Color(0,0,1,1));

    int halfSlices = slices/2;
    Color c(1,1,1,1);

        
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            
            if (i == 0)
            {
                
                c.r = 0.5f;
                c.g = 0.5f;
                c.b = 0.5f;
                


            }
            else
            {
                 c.r = 0.75f;
                c.g = 0.75f;
                c.b = 0.75f;
            }


                DrawLine3D( (float)i*spacing, 0.0f, (float)-halfSlices*spacing, (float)i*spacing, 0.0f, (float)halfSlices*spacing, c);
                DrawLine3D((float)-halfSlices*spacing, 0.0f, (float)i*spacing,(float)halfSlices*spacing, 0.0f, (float)i*spacing,  c);
            
        }
    
    
    
}

void RenderBatch::Set2D()
{
    glDisable(GL_DEPTH_TEST);    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderBatch::Set3D()
{
       glEnable(GL_DEPTH_TEST);
       glDisable(GL_BLEND);

}

void RenderBatch::DrawTexture(Texture *texture, float x, float y, float width, float height, const Rectangle &clip, const Color &color)
{

    if (!texture) return;
    if (texture->id == 0) return;

    float fx2 = x + width;
    float fy2 = y + height;

    float left, right, top, bottom;

    



    int widthTex  = texture->width;
    int heightTex = texture->height;



    left = clip.x / widthTex;
    right  = (clip.x + clip.width) / widthTex;
    top = clip.y / heightTex;
    bottom = (clip.y + clip.height) / heightTex;





    SetTexture(texture->id);
    Mode(QUADS);
    Color4f(color.r, color.g, color.b, color.a);


    TexCoord2f(left, top);
    Vertex2f(x, y);

    TexCoord2f(left, bottom);
    Vertex2f(x, fy2);


    TexCoord2f(right, bottom);
    Vertex2f(fx2, fy2);

    TexCoord2f(right, top);
    Vertex2f(fx2, y);

    SetTexture(0);


}


void RenderBatch::DrawTexture(Texture *texture, float x, float y, const Rectangle &clip, const Color &color)
{
    DrawTexture(texture, x, y, clip.width,clip.height, clip, color);
}

void RenderBatch::DrawTexture(Texture *texture, float x, float y, float width, float height, const Color &color)
{
    if (!texture) return;
    Rectangle clip(0.0f, 0.0f, (float)texture->width, (float)texture->height);
    DrawTexture(texture, x, y, width, height, clip, color);
}

