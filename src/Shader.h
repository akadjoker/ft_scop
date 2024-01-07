#pragma once
#include "pch.h"


class Matrix;

class Shader
{
public:

    Shader();
    ~Shader();
    
    bool Load(const char* vertexPath, const char* fragmentPath);
    bool Create(const char* vertexCode, const char* fragmentCode);

    void use();

    void setBool(const std::string &name, bool value) ;
    void setInt(const std::string &name, int value) ;
    void setFloat(const std::string &name, float value) ;
    
    void setArray(const std::string &name, int count, float* value) ;

    void setFloat2 (const std::string &name,float x, float y ) ;
    void setFloat3 (const std::string &name,float x, float y, float z) ;
    void setFloat4 (const std::string &name,float x, float y, float z, float w) ;

    void setMatrix (const std::string &name, const Matrix& matrix) ;

    bool BindUniform(const std::string &name);
    bool ContainsUniform(const std::string &name);



private:
    Uint32 m_id;    
    std::unordered_map<std::string, int> m_uniforms;
    int UniformLocation(const std::string &name);
};
