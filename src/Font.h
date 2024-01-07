#pragma once
#include "pch.h"
#include "Math.h"

class RenderBatch;
class Texture;


class Font
{

    public:
        Font();
        ~Font();

        bool Load(const std::string& filePath);

        void DrawText(RenderBatch* batch, const char *string, const Vector2& position,  const Color& color);
        void DrawText(RenderBatch* batch, const char *string, float x, float y,  const Color& color);
        void DrawText(RenderBatch *batch, float x, float y,  const Color &color, const char *text, ...);

        void DrawText(RenderBatch *batch, const char *text, const Vector2 &position, bool center_x,bool center_y, const Color &color);
        void DrawText(RenderBatch *batch, float x, float y, bool center_x,bool center_y,  const Color &color, const char *text, ...);


        int GetWidth(const char *string);
        int GetHeight() const { return m_height; }
        int GetLength() const { return m_length; }
 

    private:
        struct Character
        {
            char c;
            int id;
            Uint16 x;
            Uint16 y;
            Uint16 width;
            Uint16 height;
            Uint16 xoffset;
            Uint16 yoffset;
          };
        std::vector<Character> m_letters;
        std::vector<Uint16> m_sizes;
        Uint16 m_height;
        Uint16 m_length;
        Texture *m_texture;

};