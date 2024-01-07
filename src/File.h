#pragma once
#include "pch.h"


class File 
{
    public:
        File(const std::string& filePath, const std::string &mode);
        ~File();

    void Close();

    Uint32 Read(void* buffer, Uint32 size);
    Uint32 Write(const void* buffer, Uint32 size);

    Uint8 ReadUint8();
    Uint16 ReadUint16();
    Uint32 ReadUint32();
    

    Uint32 Seek(long offset, bool relative = false);

    Uint32 Size() const { return m_size; }
    bool IsOpen() const { return m_open; }
    private:
        SDL_RWops* f_file;
        bool m_open;
        Uint32 m_size;
};
