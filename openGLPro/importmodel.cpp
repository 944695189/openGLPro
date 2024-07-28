#include "importmodel.h"

void imodel::initshader(const char* vsfile, const char* fsfile)
{
	ourShader.shader(vsfile, fsfile);
	
	
}
void imodel::initmodel(const char* path) 
{
	ourModel.model(path);
}
void imodel::render()
{
	ourShader.use();

	model = glm::translate(mat4(1.0f), glm::vec3(3.0f, -3.0f, .0f)); 
	model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	
	//model = glm::rotate(model, float(10.5f), glm::vec3(1.0, .0, .0));
	//model2 *= glm::rotate(mat4(1.0f), float(.2f), glm::vec3(.0, 1.0, .0));
	//model = model1 * model2;
	ourShader.SetUniform("ModelMatrix", model);
	ourShader.SetUniform("ProjectionMatrix", projection);
	ourShader.SetUniform("ViewMatrix", view);

	ourShader.SetUniform("lightColor", lightcolor);
	ourShader.SetUniform("lightPos", lightPo);
	ourShader.SetUniform("viewPos", viewPos);

	ourShader.SetUniform("Ka", Ka);
	ourShader.SetUniform("Kd", Kd);
	ourShader.SetUniform("Ks", Ks);
	ourShader.SetUniform("shiness", shiness);
	ourModel.Draw(ourShader);
}