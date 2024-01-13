#include "Utils.h"


void Log(int severity, const char* fmt, ...)
{

		const char* type;
		const char* color;
		switch (severity) 
        {
			case  0:
				type = "info";
				color = CONSOLE_COLOR_GREEN;
				break;
			case 1:
				type = "warning";
				color = CONSOLE_COLOR_PURPLE;
				break;
            case 2:
				type = "error";
				color = CONSOLE_COLOR_RED;
				break;
			default:
				break;
		}


		time_t rawTime;
		struct tm* timeInfo;
		char timeBuffer[80];

		time(&rawTime);
		timeInfo = localtime(&rawTime);

		strftime(timeBuffer, sizeof(timeBuffer), "[%H:%M:%S]", timeInfo);

		char consoleFormat[1024];
		snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN,
				timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);


		char fileFormat[1024];
		snprintf(fileFormat, 1024, "%s %s: %s\n", timeBuffer, type, fmt);

		va_list argptr;

		va_start(argptr, fmt);
		vprintf(consoleFormat, argptr);
		va_end(argptr);

		char buf[4096];

		va_start(argptr, fmt);
		vsprintf(buf, fmt, argptr);
		va_end(argptr);
}

bool FileExists(const std::string& filename) 
{
    std::ifstream file(filename);
    return file.good();
}

 char *LoadFileText(const char *fileName)
{
    char *text = NULL;

        SDL_RWops* textFile= SDL_RWFromFile(fileName, "rt");
        if (textFile != NULL)
        {
          unsigned  int size =(int) SDL_RWsize(textFile);
            if (size > 0)
            {
                text = (char *)malloc((size + 1)*sizeof(char));
                unsigned int count = (unsigned int) SDL_RWread(textFile, text, sizeof( char), size);
                if (count < size) text = (char*)realloc(text, count + 1);
                text[count] = '\0';
                Log(0, "FILEIO: [%s] Text file loaded successfully", fileName);
            }
            else Log(2, "FILEIO: [%s] Failed to read text file", fileName);

            SDL_RWclose(textFile);
        }
        else Log(2, "FILEIO: [%s] Failed to open text file", fileName);

    return text;
}


Uint32 CompileShader(const char *shaderCode, int type)
{
    Uint32 shader = 0;


    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);

    GLint success = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: Log(2, "SHADER: [ID %i] Failed to compile vertex shader code", shader); break;
            case GL_FRAGMENT_SHADER: Log(2, "SHADER: [ID %i] Failed to compile fragment shader code", shader); break;
            default: break;
        }

        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            char *log = (char *)std::calloc(maxLength, sizeof(char));
            glGetShaderInfoLog(shader, maxLength, &length, log);
            Log(2, "SHADER: [ID %i] Compile error: %s", shader, log);
            std::free(log);
        }
    }
    else
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: Log(0, "SHADER: [ID %i] Vertex shader compiled successfully", shader); break;
            case GL_FRAGMENT_SHADER: Log(0, "SHADER: [ID %i] Fragment shader compiled successfully", shader); break;
            default: break;
        }
    }
    return shader;
}

Uint32 LoadShaderProgram(Uint32 vShaderId, Uint32 fShaderId)
{
    Uint32 program = 0;
    GLint success = 0;
    program = glCreateProgram();
    glAttachShader(program, vShaderId);
    glAttachShader(program, fShaderId);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
       Log(2,"SHADER: [ID %i] Failed to link shader program", program);

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            char *log = (char *)std::calloc(maxLength, sizeof(char));
            glGetProgramInfoLog(program, maxLength, &length, log);
            Log(2, "SHADER: [ID %i] Link error: %s", program, log);
            std::free(log);
        }

        glDeleteProgram(program);

        program = 0;
    }
    else
    {
        
        Log(0, "SHADER: [ID %i] Program shader loaded successfully", program);
    }
    return program;
}


Uint32 LoadShaderFromFile(const char *vsFileName, const char *fsFileName)
{

    Uint32 vShaderId = 0;
    Uint32 fShaderId = 0;
    Uint32 id = 0;


    char *vShaderStr = NULL;
    char *fShaderStr = NULL;
    vShaderStr = LoadFileText(vsFileName);
    fShaderStr = LoadFileText(fsFileName);
    if (vShaderStr != NULL && fShaderStr != NULL)
    {
        vShaderId = CompileShader(vShaderStr, GL_VERTEX_SHADER);
        fShaderId = CompileShader(fShaderStr, GL_FRAGMENT_SHADER);
        if (vShaderId != 0 && fShaderId != 0)
        {
            id = LoadShaderProgram(vShaderId, fShaderId);
            if (id != 0)
            {
                glDeleteShader(vShaderId);
                glDeleteShader(fShaderId);
            } else 
            {
                Log(2, "SHADER: Failed to load shader from file");
            }
        }
    } else Log(2, "SHADER: Failed to load shader from file");
    if (vShaderStr != NULL) std::free(vShaderStr);
    if (fShaderStr != NULL) std::free(fShaderStr);
    return id;    
}


Uint32 LoadShaderFromMemory(const char *vShaderStr, const char *fShaderStr)
{
    
    
    Uint32 vShaderId = 0;
    Uint32 fShaderId = 0;
    Uint32 id = 0;

    vShaderId = CompileShader(vShaderStr, GL_VERTEX_SHADER);
    fShaderId = CompileShader(fShaderStr, GL_FRAGMENT_SHADER);
    if (vShaderId != 0 && fShaderId != 0)
    {
        id = LoadShaderProgram(vShaderId, fShaderId);
        if (id != 0)
        {
            glDeleteShader(vShaderId);
            glDeleteShader(fShaderId);
        } else 
        {
            Log(2, "SHADER: Failed to load shader from memory");
        }
    } else Log(2, "SHADER: Failed to load shader from memory");

    return id;
}


void GetColorFormats(PixelFormat format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType)
{
    *glInternalFormat = 0;
    *glFormat = 0;
    *glType = 0;

    switch (format)
    {
        case PixelFormat::GRAYSCALE: *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_UNSIGNED_BYTE; break;
        case PixelFormat::GRAY_ALPHA: *glInternalFormat = GL_LUMINANCE_ALPHA; *glFormat = GL_LUMINANCE_ALPHA; *glType = GL_UNSIGNED_BYTE; break;
        case PixelFormat::R8G8B8: *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_UNSIGNED_BYTE; break;
        case PixelFormat::R8G8B8A8: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_BYTE; break;
     
    }
}


Uint32 CreateTexture(const void *data, int width, int height, PixelFormat format)
{
    Uint32 id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);          
    glBindTexture(GL_TEXTURE_2D, id);


    Uint32 glInternalFormat, glFormat, glType;
    GetColorFormats(format, &glInternalFormat, &glFormat, &glType);
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
    glBindTexture(GL_TEXTURE_2D, 0);
    if (id > 0) Log(0, "TEXTURE: [ID %i] Texture loaded successfully (%ix%i) ", id, width, height);
    else Log(2,"TEXTURE: Failed to load texture");
    return id;
}

void UnloadVertexArray(Uint32 vaoId)
{
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);

}

void UnloadVertexBuffer(Uint32 vboId)
{
    glDeleteBuffers(1, &vboId);
}

void UnloadTexture(Uint32 id)
{
    glDeleteTextures(1, &id);
  
}

Uint32 GetTicks(void)
{
    return SDL_GetTicks();
}
double GetTime(void)
{
    return (double) SDL_GetTicks()/1000.0;
}



std::string GetFileExtension(const std::string& filename) 
{
    size_t dotPosition = filename.find_last_of(".");
    if (dotPosition != std::string::npos) 
    {
        return filename.substr(dotPosition + 1);
    }
    return ""; 
}

std::string GetFileName(const std::string& filepath) 
{
    size_t slashPosition = filepath.find_last_of("/\\");
    if (slashPosition != std::string::npos) {
        return filepath.substr(slashPosition + 1);
    }
    return filepath;
}

std::string GetFileNameNoExtension(const std::string& filepath) 
{
    std::string filename = GetFileName(filepath);
    size_t dotPosition = filename.find_last_of(".");
    if (dotPosition != std::string::npos) 
    {
        return filename.substr(0, dotPosition);
    }
    return filename;

}

std::string GetFilePath(const std::string& filepath) 
{
    size_t slashPosition = filepath.find_last_of("/\\");
    if (slashPosition != std::string::npos) 
    {
        return filepath.substr(0, slashPosition + 1);
    }
    return "";
    
}

std::string ToLower(const std::string& str) 
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringReplace(const std::string &haystack, const std::string &needle, const std::string &replacement)
{
    if (needle.empty()) return "";
    std::string result = haystack;
    size_t pos = 0;
    while ((pos = result.find(needle, pos)) != std::string::npos)
    {
        result.replace(pos, needle.length(), replacement);
        pos += replacement.length();
    }
    return result;
    
}


std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


std::vector<std::string> Split(const std::string &s, char delimiter) 
{
   std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    
    while (std::getline(tokenStream, token, delimiter))
    {
            if (token == "") continue;
           // Log(0, "TOKEN: %s", token.c_str());
            tokens.push_back(token);
        
    }
    
    return tokens;
}
