#include "Font.h"
#include "Utils.h"
#include "Batch.h"
#include "Texture.h"

Font::Font()
{
    m_texture = new Texture();
    m_texture->Load("resources/font2.tga",false);
    Load("resources/font2.fnt");
}

Font::~Font()
{
 
    delete m_texture;
}

bool Font::Load(const std::string& filePath)
{

       std::ifstream file(filePath);
       m_height =1;

       if (file.is_open()) 
       {
        std::string line;
        while (std::getline(file, line)) 
        {
            Character data;
            
            std::istringstream iss(line);
           // std::cout<<line<<std::endl;

            data.width= 1;
            data.height= 1;
            data.xoffset= 0;
            data.yoffset= 0;
            data.x=0;
            data.y=0;



            size_t start = line.find('"');
            size_t end = line.find('"', start + 1);
            if (start != std::string::npos && end != std::string::npos) 
            {
               data.id = line[start + 1];
               data.c = line[start + 1];
            }



             // Verificamos se a linha tem o número esperado de valores separados por delimitadores
            std::stringstream ss(line.substr(end + 1));
            char delimiter;
            if (ss >> delimiter >> data.x >> delimiter >> data.y >> delimiter >> data.width >> delimiter >> data.height >> delimiter) 
            {
            
                if (delimiter != ',' || ss.fail() || data.width <= 1 || data.height <= 1) 
                {
                    Log(1, "Formato incorreto na linha: %s",line.c_str());
                    return false;
                }



                if (data.height > m_height) 
                {
                    m_height = data.height;
                }

            // std::stringstream ss(line.substr(end + 1));
            // char delimiter;
            // ss >> delimiter >> data.x >> delimiter >> data.y >> delimiter  >> data.width >> delimiter >> data.height >> delimiter;

            // if (data.height > m_height) 
            // {
            //     m_height = data.height;
            // }
       
        //    std::cout<<data.id<< " " <<data.x<<" "<<data.y<<" "<<data.width<<" "<<data.height<<std::endl;
            m_letters.push_back(data);
            m_sizes.push_back(data.width);
            }

         
          
        }
        file.close();
        return true;
    } else 
    {
        Log(2, "Erro ao abrir o arquivo %s ", filePath.c_str());
        return false;
    }
    
}

void Font::DrawText(RenderBatch *batch, const char *text, const Vector2 &position,  const Color &color)
{
    float fx = position.x;
    float fy = position.y;
    float proportion = 1.0f;
    float scale = 1.0f;
    float max_width=1;
    float total =0;


  while (*text) 
  {
    if (*text == '\n') 
    {
      fy += int(m_height * scale);
      fx = position.x;
      
    } else 
    if (*text == ' ') 
    {
      fx += int(max_width * scale);
    } else
    {
        Uint32 i = static_cast<unsigned char>(*text) - 33;
        fx +=  proportion* scale;
        Rectangle clip;
        
        clip.x = m_letters[i].x;
        clip.y = m_letters[i].y;
        clip.width = m_letters[i].width;
        clip.height = m_letters[i].height;

        if (max_width < clip.width)
        {
            max_width = clip.width;
        }
        batch->DrawTexture(m_texture, fx, fy,  clip, color);
        
        total += (clip.width ) * scale * proportion;

        

        fx += (clip.width ) * scale * proportion;
      
    }
    text++;
  }

  if (total > m_length)
  {
      m_length = total;
  }
}


void Font::DrawText(RenderBatch *batch, const char *text, const Vector2 &position, bool center_x,bool center_y, const Color &color)
{
    float fx = position.x;
    float fy = position.y;
    float proportion = 1.0f;
    float scale = 1.0f;
    float max_width=1;


    if (center_x  || center_y)
    {
        float width = GetWidth(text);
        float height = m_height;
        if (center_x)
        {
            fx -= width / 2;
        }
        if (center_y)
        {
            fy -= height / 2;
        }
    }
    

  while (*text) 
  {
    if (*text == '\n') 
    {
      fy += int(m_height * scale);
      fx = position.x;
      
    } else 
    if (*text == ' ') 
    {
      fx += int(max_width * scale);
    } else
    {
        Uint32 i = static_cast<unsigned char>(*text) - 33;
        fx +=  proportion* scale;
        Rectangle clip;
        clip.x = m_letters[i].x;
        clip.y = m_letters[i].y;
        clip.width = m_letters[i].width;
        clip.height = m_letters[i].height;

        if (max_width < clip.width)
        {
            max_width = clip.width;
        }
        batch->DrawTexture(m_texture, fx, fy,  clip, color);

        

        fx += (clip.width ) * scale * proportion;
      
    }
    text++;
  }

     
}
void Font::DrawText(RenderBatch *batch, float x, float y,  bool center_x,bool center_y,const Color &color, const char *text, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    DrawText(batch, buffer, Vector2(x, y), center_x,center_y, color);
}
void Font::DrawText(RenderBatch *batch, const char *string, float x, float y, const Color &color)
{
    DrawText(batch, string, Vector2(x, y), color);
}

void Font::DrawText(RenderBatch *batch, float x, float y, const Color &color, const char *text, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    DrawText(batch, buffer, Vector2(x, y), color);
}

int Font::GetWidth(const char *text)
{
  Uint16 width =0;
  Uint32 n;
	for(const char* p = text; *p; ++p)
	{
		n = (*p) - 33;
		if ( n > (Uint32)m_sizes.size())
			  n = 0;
		width += (int)m_sizes[n];
	}
  return width;

  //   float proportion = 1.0f;
  //   float scale = 1.0f;
  //   float fx = 0; 
  //   float max_width = 0;
  //   while (*text) 
  //   {
  //     if (*text == '\n') 
  //     {
  //       fx += max_width * scale * proportion;
  //     } else 
  //     if (*text == ' ') 
  //     {
  //       fx += int(max_width * scale);
  //     } else
  //     {
  //         Uint32 i = (*text) - 32;
  //         fx +=  proportion * scale;
  //         fx += (m_letters[i]->width) * scale * proportion;
  //         if (max_width < m_letters[i]->width)
  //             max_width = m_letters[i]->width;
        
  //     }
  //     text++;
  //   }
  // return fx +1;
}
