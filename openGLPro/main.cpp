#pragma once
#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#include "sky.h"
#include"Sphere.h"
#include"importmodel.h"
//#include<ctime>

using namespace std;

#define numVAOs 1
#define numVBOs 4

//time_t starttime = std::time(nullptr);
float radius = .5f;//球体半径
//bool loc;
//float alpha=0;
double lastX = 0.0;
double lastY = 0.0;
bool MousePressed = false;
double Zpos = 0.0;
Sphere sphere;
int rendermode = 0;
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
float cameraHeight = 0.0f, cameraHorizontal = 200.0f, cameraVertial=1.0f,aspect;

GLuint vao[numVAOs], vbo[numVBOs];


glm::mat4 pMat, vMat;
//天空盒纹理
GLuint skyboxTexture;
Shader rProgramSky;
imodel tmodel;

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
	//if (action == GLFW_PRESS || action == GLFW_REPEAT) {
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
		// 抬升相机
		pos[1] += 0.30f;
		break;
	case GLFW_KEY_X:
		// 降低相机
		pos[1] -= 0.30f;
		break;
	case GLFW_KEY_T:
		rendermode = (++rendermode) % 2;
		break;
	case GLFW_KEY_1:
		lightPo[0] += 0.1f;
		break;
	case GLFW_KEY_2:
		lightPo[0] -= 0.1f;
		break;
	default:
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

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
}

//绘制天空盒模型数据更新
void prepForSkyBoxRender() {
	rProgramSky.use();
	rProgramSky.SetUniform("v_matrix", vMat);
	rProgramSky.SetUniform("p_matrix", pMat);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	//GL_TEXTURE_CUBE_MAP是一个纹理目标，表示立方体贴图

}

void init(GLFWwindow* window) {

	rProgramSky.shader("shader/vShaderCube.vs", "shader/fShaderCube.fs");
	initVertices();
	skyboxTexture = Utils::loadCubeMap("cubeMap/skybox3");
	sphere.InitData(radius);
	sphere.LoadShader("shader/tplight.vs", "shader/tplight.fs");
	sphere.SetTexture(Utils::loadTexture("Textures/1712417437099.jpg"));
	tmodel.initshader("shader/tplight.vs", "shader/tplight.fs");
	tmodel.initmodel("AmberDoll/Doll.pmx");
}


void display(GLFWwindow* window, double currentTime) {
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(static_cast<float>DEG2RAD(60.0f), aspect, 0.1f, 400.0f);
	//glm::perspective用于创建投影矩阵,用于将三维场景投影到二维视图上
	sphere.projectionMat = pMat;
	//vMat = mat4(1.0f);
	//vMat= glm::lookAt(pos, at, glm::vec3(1.0f, 1.0f, 1.0f));
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, cameraHeight, Zpos))
	     *glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraHorizontal), glm::vec3(.0f, 1.0f, .0f))
		*glm::rotate(glm::mat4(1.0f),static_cast<float>DEG2RAD(cameraVertial),glm::vec3(1.0f,0.0f,.0f));
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prepForSkyBoxRender();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	for (int i = 0; i < 3; i++)
	{
		at[i] = pos[i] - headdir[i];
	}
	
		sphere.viewMat = glm::lookAt(pos, at, glm::vec3(.0f, 1.0f, .0f));
		// 设置多边形填充模式
		if (rendermode == 1)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_DEPTH_TEST);

		sphere.Render();
	tmodel.render();
	tmodel.view = sphere.viewMat;
	tmodel.projection = pMat;
	
}

void size(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
		
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }//初始化glfw
	
	GLFWwindow* window = glfwCreateWindow(1300,800, "Skyboxandmodel", NULL, NULL);
	glfwMakeContextCurrent(window);//将window设置为opengl上下文，即opengl在这里绘图
	glfwSetFramebufferSizeCallback(window, size);//窗口大小变化
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);//用于设置垂直同步
	//参数 1 表示打开垂直同步，意味着图形帧缓冲的刷新率将与显示器的垂直刷新率同步。这通常被用来避免屏幕撕裂(Tearing)，提升图形渲染的质量。
	
	init(window);
	
	glfwSetMouseButtonCallback(window, mouse_button_callback);//鼠标按下与否监听
	
	glfwSetCursorPosCallback(window, mouse_callback);//光标位置监听
	glfwSetKeyCallback(window, keyboard);
	while (!glfwWindowShouldClose(window)) {//窗口关闭则推出循环
		
		display(window, glfwGetTime());

		glfwSwapBuffers(window);//双缓冲模式避免闪烁、不完整
		glfwPollEvents();
		// 这一行用于处理窗口事件。例如，当用户移动窗口、调整窗口大小、按下键盘或移动鼠标时，这些事件会被记录到事件队列中。
		//调用 glfwPollEvents() 函数会处理这些事件并触发相应的回调函数。
	}

	glfwDestroyWindow(window);
	glfwTerminate();//清理glfw状态，释放资源
	exit(1);
}
