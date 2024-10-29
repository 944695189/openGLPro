#pragma once
#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <string>
#include <iostream>
#include <fstream>// fstream是C++ STL中对文件操作的合集，包含了常用的所有文件操作
#include<vector>
#include <math.h>

#define DEG2RAD(theta)		((theta * 2.0 * 3.14159)/360.0)


class sky
{
public:

	sky() {}
	static GLuint loadTexture(const char *texImagePath);//静态成员函数可通过Utils::loadTexture形式访问
	static GLuint loadCubeMap(const char *mapDir);
};
