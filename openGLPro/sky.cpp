#pragma once
#include "sky.h"
using std::string;
using std::cout;
using std::endl;
//º”‘ÿÕº∆¨
//Utils::Utils() {}

GLuint Utils::loadCubeMap(const char* mapDir)
{
	GLuint textureRef;
	string xp = mapDir; xp = xp + "/posx.jpg";
	string xn = mapDir; xn = xn + "/negx.jpg";
	string yp = mapDir; yp = yp + "/posy.jpg";
	string yn = mapDir; yn = yn + "/negy.jpg";
	string zp = mapDir; zp = zp + "/posz.jpg";
	string zn = mapDir; zn = zn + "/negz.jpg";
	/*string xp = mapDir; xp = xp + "/right.jpg";
	string xn = mapDir; xn = xn + "/left.jpg";
	string yp = mapDir; yp = yp + "/top.jpg";
	string yn = mapDir; yn = yn + "/bottom.jpg";
	string zp = mapDir; zp = zp + "/front.jpg";
	string zn = mapDir; zn = zn + "/back.jpg";*/
	textureRef = SOIL_load_OGL_cubemap(xp.c_str(), xn.c_str(), yp.c_str(), yn.c_str(), zp.c_str(), zn.c_str(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (textureRef == 0) cout << "didnt find cube map image file" << endl;
	return textureRef;
}

GLuint Utils::loadTexture(const char* texImagePath)
{
	GLuint textureRef;
	textureRef = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureRef == 0) cout << "didnt find texture file " << texImagePath << endl;
	// ----- mipmap/anisotropic section
	glBindTexture(GL_TEXTURE_2D, textureRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoset = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoset);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoset);
	}
	// ----- end of mipmap/anisotropic section
	return textureRef;
}

