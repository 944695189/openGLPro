#pragma once
#include "Sphere.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include<iostream>
using glm::vec4;
using namespace std;
#define PI 3.14259
extern vec3 lightPo = glm::vec3(1, -1, 1);
extern vec3 viewPos = glm::vec3(7, 20, 1);

extern vec3 lightcolor = glm::vec3(1, 1, 1);

extern vec3 Ka = vec3(0.5f, 0.5f, 0.5f);//环境光
extern vec3 Kd = vec3(0.5f, .5f, .5f);//漫反射光
extern vec3 Ks = vec3(1.0f, 1.0f, 1.0f);//镜面反射光

extern int shiness = 20;//高光系数

Sphere::Sphere(void)
{

}


Sphere::~Sphere(void)
{
	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象
}



void Sphere::InitData(float r)
{
	fR = r;

	//vTris.clear();
	//生成球心在坐标原点的球面三角网
	int m = 40;//水平方向圆离散化多边形边数
	int n = 40;//垂直方向切片个数

	iCount = m * n * 2;
	//定义顶点数组
	float* vertc = new float[3 * 3 * iCount];
	float* normal = new float[3 * 3 * iCount];//法向向量
	float* textcor = new float[3 * 2 * iCount];//纹理坐标

	float dalpha = 2 * PI / m;
	float dbeta = PI / n;

	float beta = -PI / 2.0f;
	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	vec3 vnormal;

	for (int i = 0; i < n; i++)
	{
		float alpha = 0.0f;

		float z1 = fR * sin(beta);
		float z2 = fR * sin(beta + dbeta);

		float r0 = fR * cos(beta);
		float r1 = fR * cos(beta + dbeta);

		float t1 = beta / PI + 0.5f;
		float t2 = (beta + dbeta) / PI + 0.5f;

		beta += dbeta;
		for (int j = 0; j < m; j++)
		{
			float x01 = r0 * cos(alpha);
			float y01 = r0 * sin(alpha);

			float x02 = r0 * cos(alpha + dalpha);
			float y02 = r0 * sin(alpha + dalpha);

			float x11 = r1 * cos(alpha);
			float y11 = r1 * sin(alpha);

			float x12 = r1 * cos(alpha + dalpha);
			float y12 = r1 * sin(alpha + dalpha);

			float s1 = alpha / (2.0f * PI);
			float s2 = (alpha + dalpha) / (2.0f * PI);
			//四边形分成2个三角形
			//构建第1个三角形
			vertc[vindex++] = x01; vertc[vindex++] = y01; vertc[vindex++] = z1;
			//法向量
			vnormal = glm::normalize(vec3(x01, y01, z1));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			//纹理坐标
			textcor[tindex++] = s1; textcor[tindex++] = t1;

			vertc[vindex++] = x02; vertc[vindex++] = y02; vertc[vindex++] = z1;
			vnormal = glm::normalize(vec3(x02, y02, z1));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t1;

			vertc[vindex++] = x11; vertc[vindex++] = y11; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x11, y11, z2));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s1; textcor[tindex++] = t2;

			//第2个三角形
			vertc[vindex++] = x02; vertc[vindex++] = y02; vertc[vindex++] = z1;
			vnormal = glm::normalize(vec3(x02, y02, z1));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t1;

			vertc[vindex++] = x12; vertc[vindex++] = y12; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x12, y12, z2));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t2;

			vertc[vindex++] = x11; vertc[vindex++] = y11; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x11, y11, z2));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s1; textcor[tindex++] = t2;

			alpha += dalpha;
		}
	}
	glGenBuffers(3, vboHandle);//生成缓冲区，数据
	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), vertc, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（法向量）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), normal, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点法向量输入（序号2，法向量）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量2读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//缓冲区数据（纹理）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * iCount * sizeof(float), textcor, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点纹理输入（序号1，纹理）属性对应
	//(GLuint)2: 指定顶点属性的索引为2
	glEnableVertexAttribArray(2);  //2为顶点属性索引  // 允许Vertex着色器中输入变量1读取显存数据。

	

	glBindVertexArray(0);

	delete[]vertc;
	delete[]textcor;
	delete[]normal;
	
}

void Sphere::SetTexture(GLuint texture)
{
	textureID = texture;
}

void Sphere::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.shader(vsfileName, fsfileName);
}

void Sphere::Render()
{
	//启用需要的shader，可以动态切换不同的shader
	
	
	prog.use();
	////model在头文件里定义了
	glm::mat4 model1 = mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, .0f, .0f));
	model = glm::rotate(model, float(.01f), glm::vec3(.0, 1.0, .0));
	model= glm::translate(model, glm::vec3(-3.0f, .0f, .0f));
	model1 = glm::rotate(mat4(1.0f), float(.05f), glm::vec3(1.0, .0, .0));
	model = model1*model;
	//model *= model2;
	prog.SetUniform("ProjectionMatrix", projectionMat);
	prog.SetUniform("ViewMatrix", viewMat);
	prog .SetUniform("ModelMatrix", model);//ModelMatrix、ViewMatrix和ProjectionMatrix都是在shader中定义的uniform mat4 变量
	//model更多意义上是初始化作用，将model传给ModelMatrix，ModelMatrix在于shader中获取的的顶点数据等计算

	
	prog.SetUniform("lightColor", lightcolor);
	prog.SetUniform("lightPos", lightPo);
	prog.SetUniform("viewPos", viewPos);

	prog.SetUniform("Ka", Ka);
	prog.SetUniform("Kd", Kd);
	prog.SetUniform("Ks", Ks);
	prog.SetUniform("shiness", shiness);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3 * iCount);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);


	//关闭所用的shader
	prog.Unuse();
	
}


