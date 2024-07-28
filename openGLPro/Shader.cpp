#pragma once
#include "Shader.h"
void Shader::shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::use()
{
    glUseProgram(ID);
}
int Shader::GetUniformLocation(const char* name)
{
	std::map<string, int>::iterator pos;
	pos = m_vUniformLocations.find(name);

	if (pos == m_vUniformLocations.end())
	{
		m_vUniformLocations[name] = glGetUniformLocation(ID, name);

	}

	return m_vUniformLocations[name];
}
void Shader::SetUniform(const char* name, float x, float y, float z)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, x, y, z);
}

void Shader::SetUniform(const char* name, const vec3& v)
{
	this->SetUniform(name, v.x, v.y, v.z);
}

void Shader::SetUniform(const char* name, const vec4& v)
{
	GLint loc = GetUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const char* name, const vec2& v)
{
	GLint loc = GetUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void Shader::SetUniform(const char* name, const mat4& m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const char* name, const mat3& m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const char* name, float val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1f(loc, val);
}

void Shader::SetUniform(const char* name, int val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1i(loc, val);
}

void Shader::SetUniform(const char* name, GLuint val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1ui(loc, val);
}

void Shader::SetUniform(const char* name, bool val)
{
	int loc = GetUniformLocation(name);
	glUniform1i(loc, val);
} 
void Shader::Unuse()
{
    glUseProgram(0);
}
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}