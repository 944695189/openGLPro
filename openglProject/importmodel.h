#pragma once
#include"model.h"
#include"Sphere.h"
class imodel {
public:
	imodel() {}
	void initshader(const char* vsfile, const char* fsfile);
	void initmodel(const char* path);
	void render();

	Shader ourShader;
	Model ourModel;
	glm::mat4 model = mat4(1.0f);
	glm::mat4 model1 = mat4(1.0f);
	glm::mat4 model2 = mat4(1.0f);
	glm::mat4 projection ;
	glm::mat4 view;

};