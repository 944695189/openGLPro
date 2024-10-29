#pragma once
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glad/glad.h>

#include "gl/glew.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include<vector>

using std::string;
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
class Shader
{
public:
    std::map<string, int> m_vUniformLocations;

    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader() {}
    void shader(const char* vertexPath, const char* fragmentPath);
    // activate the shader
    // ------------------------------------------------------------------------
    void use();
    GLint  GetUniformLocation(const char* name);
    void   SetUniform(const char* name, float x, float y, float z);
    void   SetUniform(const char* name, const vec2& v);
    void   SetUniform(const char* name, const vec3& v);
    void   SetUniform(const char* name, const vec4& v);
    void   SetUniform(const char* name, const mat4& m);
    void   SetUniform(const char* name, const mat3& m);
    void   SetUniform(const char* name, float val);
    void   SetUniform(const char* name, int val);
    void   SetUniform(const char* name, bool val);
    void   SetUniform(const char* name, GLuint val);

    void Unuse();
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};
