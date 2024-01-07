#include "Shader.h"    
#include "Utils.h"
#include "Math.h"


Shader::Shader(): m_id(0)
{

}

Shader::~Shader()
{
    if (m_id != 0)
        glDeleteProgram(m_id);
}

bool Shader::Load(const  char *vertexPath, const char *fragmentPath)
{
    
    m_id = LoadShaderFromFile(vertexPath, fragmentPath);
    if (m_id == 0)
        return false;
    return true;

}

bool Shader::Create(const char *vertexCode, const char *fragmentCode)
{
    m_id = LoadShaderFromMemory(vertexCode, fragmentCode);
    if (m_id == 0)
        return false;
    return true;

}

void Shader::use()
{
    if (m_id != 0)
        glUseProgram(m_id);
}

void Shader::setBool(const std::string &name, bool value) 
{
    if (m_id == 0)
        return;
    int location = UniformLocation(name);
    if (location == -1)        return;
    glUniform1i(location, (int)value);
}


void Shader::setInt(const std::string &name, int value) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    glUniform1f(location, value);
}



void Shader::setFloat2(const std::string &name, float x, float y) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    
    glUniform2f(location, x, y);
}

void Shader::setFloat3(const std::string &name, float x, float y, float z) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    
    glUniform3f(location, x, y, z);
}

void Shader::setFloat4(const std::string &name, float x, float y, float z, float w) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    
    glUniform4f(location, x, y, z, w);
}

void Shader::setMatrix(const std::string &name, const Matrix &m) 
{
    if (m_id == 0)
        return;
    int location =UniformLocation(name);
    if (location == -1)        return;
    

      Matrix matrix = m;
      float mat[16]=
            {
                    matrix.m0, matrix.m1, matrix.m2, matrix.m3,
                    matrix.m4, matrix.m5, matrix.m6, matrix.m7,
                    matrix.m8, matrix.m9, matrix.m10, matrix.m11,
                    matrix.m12, matrix.m13, matrix.m14, matrix.m15
            };
    glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

bool Shader::BindUniform(const std::string &name)
{
    if (m_id == 0)
    {
        Log(2, "Shader not loaded");
        return false;
    }
    if (m_uniforms.find(name) != m_uniforms.end())
        return true;
    int location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1)
    {
        Log(2, "Uniform %s not found", name);
        return false;
    }
    m_uniforms[name] = location;
    return true;
 
}

bool Shader::ContainsUniform(const std::string &name)
{
    if (m_id == 0)
    {
        Log(2, "Shader not loaded");
        return false;
    }
    if (m_uniforms.find(name) != m_uniforms.end())
        return true;
    int location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1)
    {
        Log(2, "Uniform %s not found", name);
        return false;
    }
    m_uniforms[name] = location;
    return true;

}

int Shader::UniformLocation(const std::string &name)
{
    if (m_id == 0)
    {
        Log(2, "Shader not loaded");
        return -1;
    }
   
    int location = m_uniforms[name];
    if (location == -1)
    {
        Log(2, "Uniform %s not found", name);
        return -1;
    }
    return location;
}
