#pragma once
#include<vector>
#include <GL\glew.h>
#include"Shader.h"
#include"sky.h"
using namespace std;

extern vec3 lightPo;

extern vec3 viewPos;
extern vec3 lightcolor;

extern vec3 Ka;//环境光
extern vec3 Kd;//漫反射光
extern vec3 Ks;//镜面反射光

extern int shiness;//高光系数
class Sphere
{
public:
	Sphere(void);
	~Sphere(void);

	void InitData(float r);
	void Render();

	void SetTexture(GLuint texture);

	void LoadShader(const char* vsfileName, const char* fsfileName);
	//const char* vsfile=nullptr, *fsfile=nullptr;
	Shader prog;
	//	cgProgram textprog;
	//cgProgram lihgtprog;
	unsigned int vboHandle[3];
	unsigned int vaoHandle;

	mat4 projectionMat = glm::mat4(1.0f);
	mat4 viewMat = glm::mat4(1.0f);
	mat4 model = glm::mat4(1.0f);
	


	

private:
	float fR;

	GLuint textureID;

	int iCount;//三角形个数
};

