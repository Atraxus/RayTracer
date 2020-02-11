#include "ComputeShader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../Renderer/Renderer.h"


ComputeShader::ComputeShader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    std::string source = ParseShader(filepath);
    m_RendererID = CreateShader(source);
}

ComputeShader::~ComputeShader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void ComputeShader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ComputeShader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void ComputeShader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void ComputeShader::SetUniform1ui(const std::string& name, unsigned int value)
{
	GLCall(glUniform1ui(GetUniformLocation(name), value));
}

void ComputeShader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void ComputeShader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void ComputeShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ComputeShader::SetUniform4Matf(const std::string& name, glm::mat4 proj)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(proj)));
}

int ComputeShader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

std::string ComputeShader::ParseShader(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::stringstream stream;

    stream << inFile.rdbuf();
    return stream.str();
}

unsigned int ComputeShader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr)); // nullptr because src is '\0' terminated
    GLCall(glCompileShader(id)); // read doc

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // i for int and v for vector (in this case a pointer)
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // allocate on the stack dynamically instead of the heap --- this shouldnt get to big as it can blow out the stack
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile compute shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

// parameters here are supposed to be the source code of the shaders
unsigned int ComputeShader::CreateShader(const std::string& source)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int shader = CompileShader(GL_COMPUTE_SHADER, source);

    GLCall(glAttachShader(program, shader));
    GLCall(glLinkProgram(program)); // read doc 
    GLCall(glValidateProgram(program)); // read doc

    // shaders are stored in program now and intermediaries can be deleted
    GLCall(glDeleteShader(shader));

    return program;
}