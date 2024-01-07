#include "File.h"
#include "Utils.h"

File::File(const std::string &filePath, const std::string &mode)
{
    f_file = SDL_RWFromFile(filePath.c_str(), mode.c_str());
    if (f_file == nullptr)
    {
        Log(1, "Erro ao abrir o arquivo: %s", filePath.c_str());
        m_open = false;
        return;
    }

    SDL_RWseek(f_file, 0, RW_SEEK_END);
    m_size = SDL_RWtell(f_file);
    SDL_RWseek(f_file, 0, RW_SEEK_SET);
    m_open = true;
    Log(0, "FILE: file  read: %s", filePath.c_str());
}

File::~File()
{
   Close();
}

void File::Close()
{
    if (f_file != nullptr)
    {
        Log(0, "FILE: file closed");
        SDL_RWclose(f_file);
        f_file = nullptr;
    }
}

Uint32 File::Read(void *buffer, Uint32 size)
{
    return SDL_RWread(f_file, buffer, 1, size);
}

Uint32 File::Write(const void *buffer, Uint32 size)
{
    return SDL_RWwrite(f_file, buffer, 1, size);
}

Uint8 File::ReadUint8()
{

    Uint8 value;
    SDL_RWread(f_file, &value, 1, 1);
    return value;
}

Uint16 File::ReadUint16()
{
    Uint16 value;
    SDL_RWread(f_file, &value, 2, 1);
    return value;
}

Uint32 File::ReadUint32()
{
    Uint32 value;
    SDL_RWread(f_file, &value, 4, 1);
    return value;
}

Uint32 File::Seek(long offset, bool relative)
{
    return SDL_RWseek(f_file, offset, relative ? RW_SEEK_CUR : RW_SEEK_SET);
}
