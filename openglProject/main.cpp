#pragma once
#include "sky.h"
#include"Sphere.h"
#include"importmodel.h"
#include "noisePattern.h"
using namespace std;

#define numVAOs 1
#define numVBOs 4
noisePattern noise = noisePattern();
float radius = .5f;//球体半径
//bool loc;
//float alpha=0;
double lastX = 0.0;
double lastY = 0.0;
bool MousePressed = false;
double Zpos = 0.0;
Sphere sphere;
GLuint textureID;
vec3 pos = vec3(3.0f, -1.50f, 6.0f);//相机位置
vec3 at = vec3(.0f, 0.0f, 0.0f);
float headdir[] = { 0.0f,.0f,1.0f };
float rightdir[] = {1.0f,.0f,0.0f };

float step = 0.10f;
float beta = 180.0f;//与z轴正向夹角
//显示区域宽高
int width, height;
//相机高度(水上/水下)、相机视场角、显示区域宽高比例
float cameraHeight = 2.0f, cameraHorizontal = 15.0f, cameraVertial= -10.0f,aspect;

//水面平面高度、水底平面高度
float surfacePlaneHeight = 0.0f, floorPlaneHeight = -10.0f;
GLuint vao[numVAOs], vbo[numVBOs];

//天空盒纹理、3D噪声纹理
GLuint skyboxTexture, noiseTexture;

Shader rProgramSky;
Shader rProgramSurface;
Shader rProgramFloor;
imodel tmodel;
//本地投影矩阵、视图矩阵、模型矩阵、mv矩阵、mv逆矩阵
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
//点光源位置x,y,z
float lightPos[3];
//shader Uniform：全局环境光、灯源环境光 漫反射、高光、光照位置、
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc;
//shader Uniform：材质环境光、材质漫反射、材质高光、材质光泽度、位于水面上/水面下
GLuint mambLoc, mdiffLoc, mspecLoc, mshiLoc, aboveLoc;
//点光源位置、当前光源位置、光源转换
glm::vec3 lightLoc = glm::vec3(0.0f, 4.0f, -8.0f), currentLightPos, transformed;
//白光
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//材质
float matAmb[4] = { 0.5f, 0.6f, 0.8f, 1.0f };
float matDif[4] = { 0.8f, 0.9f, 1.0f, 1.0f };
float matSpe[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float matShi = 100.0f;

//折射、反射ID;折射、反射Buffer
GLuint refractTextureId;
GLuint reflectTextureId;
GLuint refractFrameBuffer;
GLuint reflectFrameBuffer;

//3D纹理噪声动态变化调整
float depthLookup = 0.0f;
GLuint dOffsetLoc;
double prevTime;


//初始化噪声Texture
GLuint initNoiseTexture() {
	GLuint textureID;
	int noiseH = noise.getHeight(), noiseW = noise.getWidth(), noiseD = noise.getDeep();
	GLubyte* data = new GLubyte[noiseH * noiseW * noiseD * 4];
	noise.fillDataArray(data);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, noiseW, noiseH, noiseD);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseW, noiseH, noiseD, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
	delete[] data;
	return textureID;
}


//初始化光照
void installLights(glm::mat4 vMatrix, GLuint renderingProgram) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		MousePressed = true;
		glfwGetCursorPos(window, &lastX, &lastY);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		MousePressed = false;
	}
}
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		// 前移
		for (int i = 0; i < 3; i++)
			pos[i] += step * headdir[i];
		break;
	case GLFW_KEY_S:
		// 后移
		for (int i = 0; i < 3; i++)
			pos[i] -= step * headdir[i];
		break;
	case GLFW_KEY_A:
		// 左移
		for (int i = 0; i < 3; i++)
			pos[i] += step * rightdir[i];
		break;
	case GLFW_KEY_D:
		// 右移
		for (int i = 0; i < 3; i++)
			pos[i] -= step * rightdir[i];
		break;

	case GLFW_KEY_Z:
		// 抬升物体
		pos[1] += 0.30f;
		break;
	case GLFW_KEY_X:
		// 降低物体
		pos[1] -= 0.30f;
		break;
	case GLFW_KEY_1://调整点光源位置
		lightPo[0] += 0.1f;
		break;
	case GLFW_KEY_2:
		lightPo[0] -= 0.1f;
		break;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!MousePressed)
		return;
	if (lastX == 0.0 && lastY == 0.0) {
		lastX = xpos;
		lastY = ypos;
		return;
	}

	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraHorizontal += xoffset;
	cameraVertial += yoffset;


}
//初始化模型顶点数据
void initVertices() {
	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float PLANE_POSITIONS[18] = {
			-120.0f, 0.0f, -240.0f,  -120.0f, 0.0f, 0.0f,  120.0f, 0.0f, -240.0f,
			120.0f, 0.0f, -240.0f,  -120.0f, 0.0f, 0.0f,  120.0f, 0.0f, 0.0f
	};
	float PLANE_TEXCOORDS[12] = {
		0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
		1.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f
	};
	float PLANE_NORMALS[18] = {
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_POSITIONS), PLANE_POSITIONS, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_TEXCOORDS), PLANE_TEXCOORDS, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_NORMALS), PLANE_NORMALS, GL_STATIC_DRAW);
}

//创建反射和折射buffer
void createReflectRefractBuffers(GLFWwindow* window) {
	GLuint bufferId[1];
	glGenBuffers(1, bufferId);
	glfwGetFramebufferSize(window, &width, &height);

	//创建反射自定义帧缓冲区
	glGenFramebuffers(1, bufferId);
	reflectFrameBuffer = bufferId[0];
	//启用自定义一帧缓冲区
	glBindFramebuffer(GL_FRAMEBUFFER, reflectFrameBuffer);

	glGenTextures(1, bufferId);
	reflectTextureId = bufferId[0];
	glBindTexture(GL_TEXTURE_2D, reflectTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//创建颜色纹理并将其绑定到自定义缓冲区中
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectTextureId, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//创建折射自定义帧缓冲区
	glGenFramebuffers(1, bufferId);
	refractFrameBuffer = bufferId[0];
	//启用自定义一帧缓冲区
	glBindFramebuffer(GL_FRAMEBUFFER, refractFrameBuffer);
	glGenTextures(1, bufferId);
	refractTextureId = bufferId[0];
	glBindTexture(GL_TEXTURE_2D, refractTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//创建颜色纹理并将其绑定到自定义缓冲区中
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractTextureId, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
void init(GLFWwindow* window) {

	rProgramSky.shader("shader/vShaderCube.glsl", "shader/fShaderCube.glsl");
	rProgramSurface.shader("shader/vShaderSurface.glsl", "shader/fShaderSurface.glsl");
	rProgramFloor.shader("shader/vShaderFloor.glsl", "shader/fShaderFloor.glsl");
	//renderingProgramCubeMap = Utils::createShaderProgram("shader/vShaderCube.glsl", "shader/fShaderCube.glsl");
	initVertices();
	lightLoc = glm::vec3(0.0f, 5.0f, -30.0f);
	skyboxTexture = sky::loadCubeMap("asset/cubeMap");
	sphere.InitData(radius);
	sphere.LoadShader("shader/tplight.vs", "shader/tplight.fs");
	sphere.SetTexture(sky::loadTexture("asset/Textures/1662781130306.jpeg"));
	tmodel.initshader("shader/tplight.vs", "shader/tplight.fs");
	tmodel.initmodel("asset/AmberDoll/Doll.pmx");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	createReflectRefractBuffers(window);
	noise.generateNoise();
	noiseTexture = initNoiseTexture();
	prevTime = glfwGetTime();
}
//绘制天空盒模型数据更新
void prepForSkyBoxRender() {
	rProgramSky.use();
	rProgramSky.SetUniform("v_matrix", vMat);
	rProgramSky.SetUniform("p_matrix", pMat);
	aboveLoc = rProgramSky.GetUniformLocation("isAbove");
	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(aboveLoc, 1);
	else
		glUniform1i(aboveLoc, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	//GL_TEXTURE_CUBE_MAP是一个纹理目标，表示立方体贴图

}
//绘制水面模型数据及texture更新
void prepForTopSurfaceRender() {
	rProgramSurface.use();
	rProgramSurface.SetUniform("mv_matrix", vMat);
	rProgramSurface.SetUniform("proj_matrix", pMat);
	rProgramSurface.SetUniform("norm_matrix", invTrMat);
	aboveLoc = rProgramSurface.GetUniformLocation("isAbove");
	
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, surfacePlaneHeight, 0.0f));
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));

	
	currentLightPos = glm::vec3(lightLoc.x, lightLoc.y, lightLoc.z);
	installLights(vMat, rProgramSurface.ID);

	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(aboveLoc, 1);
	else
		glUniform1i(aboveLoc, 0);

	dOffsetLoc = glGetUniformLocation(rProgramSurface.ID, "depthOffset");
	glUniform1f(dOffsetLoc, depthLookup);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectTextureId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractTextureId);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, noiseTexture);
}

//绘制水下模型数据及texture更新
void prepForFloorRender() {
	rProgramFloor.use();
	rProgramFloor.SetUniform("mv_matrix", vMat);
	rProgramFloor.SetUniform("proj_matrix", pMat);
	rProgramFloor.SetUniform("norm_matrix", invTrMat);

	aboveLoc = rProgramFloor.GetUniformLocation("isAbove");

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, floorPlaneHeight, 0.0f));
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));

	currentLightPos = glm::vec3(lightLoc.x, lightLoc.y, lightLoc.z);
	installLights(vMat, rProgramFloor.ID);


	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(aboveLoc, 1);
	else
		glUniform1i(aboveLoc, 0);

	dOffsetLoc = glGetUniformLocation(rProgramFloor.ID, "depthOffset");
	glUniform1f(dOffsetLoc, depthLookup);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noiseTexture);
}

void display(GLFWwindow* window, double currentTime) {
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(static_cast<float>DEG2RAD(60.0f), aspect, 0.1f, 1000.0f);
	//glm::perspective用于创建投影矩阵,用于将三维场景投影到二维视图上

	depthLookup += (currentTime - prevTime) * .05f;//动态流动效果
	prevTime = currentTime;
	//相机高度 > 水平面高度时候才有反射
	if (cameraHeight > surfacePlaneHeight) {
		//相机视图矩阵，将相机沿着y轴上移，同时延x轴旋转cameraPitch角度,物体与相机做相反的变换
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -(surfacePlaneHeight - cameraHeight), 0.0f))
			* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraHorizontal), glm::vec3(.0f, 1.0f, .0f))
			* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(-cameraVertial), glm::vec3(1.0f, 0.0f, .0f));

		//启用反射Buffer，目标结果会存到对应buffer中
		glBindFramebuffer(GL_FRAMEBUFFER, reflectFrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
		prepForSkyBoxRender();
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36); //绘制的天空盒会存储到对应buffer
		glEnable(GL_DEPTH_TEST);
	}
	////surfacePlaneHeight//水面高度，用于判断cameraHeight（相机高度）是否高于水面
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -(-(surfacePlaneHeight - cameraHeight)), Zpos))
		* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraHorizontal), glm::vec3(.0f, 1.0f, .0f))
		* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraVertial), glm::vec3(1.0f, 0.0f, .0f));
	//启用折射buffer
	glBindFramebuffer(GL_FRAMEBUFFER, refractFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//当相机高度大于水平面高度，渲染地板
	if (cameraHeight >= surfacePlaneHeight) {
		prepForFloorRender();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else {
		//相机在水面下，渲染天空盒
		prepForSkyBoxRender();
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_DEPTH_TEST);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	prepForSkyBoxRender();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_DEPTH_TEST);


	prepForTopSurfaceRender();

	glDepthFunc(GL_LEQUAL);
	if (cameraHeight >= surfacePlaneHeight)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	prepForFloorRender();
	for (int i = 0; i < 3; i++)
	{
		at[i] = pos[i] - headdir[i];
	}	
	sphere.projectionMat = pMat;	
	sphere.viewMat = glm::lookAt(pos, at, glm::vec3(.0f, 1.0f, .0f));
	sphere.Render();
	tmodel.render();
	tmodel.view = sphere.viewMat;
	tmodel.projection = pMat;
}

//void size(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//		
//}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	
	GLFWwindow* window = glfwCreateWindow(1300,800, "Skyboxandmodel", NULL, NULL);
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, size);//窗口大小变化
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	
	init(window);
	
	glfwSetMouseButtonCallback(window, mouse_button_callback);//鼠标按下与否监听
	
	glfwSetCursorPosCallback(window, mouse_callback);//光标位置监听
	glfwSetKeyCallback(window, keyboard);
	while (!glfwWindowShouldClose(window)) {
		
		display(window, glfwGetTime());

		glfwSwapBuffers(window);//双缓冲模式避免闪烁、不完整
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();//清理glfw状态，释放资源
	exit(1);
}
