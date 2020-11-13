#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Object.h"

using namespace std;

class Material
{
private:
	glm::vec3 ambient, diffuse, specular;
	float shininess;

public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

	void sendMatToShader(GLuint shader);
};

#endif