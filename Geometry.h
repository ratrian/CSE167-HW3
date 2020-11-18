#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Object.h"
#include "Node.h"

#include "Material.h"
#include "LightSource.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

class Geometry : public Node
{
private:
	glm::mat4 modelView;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> indices;

	GLuint VAO, VBO, NBO, EBO;
	
	GLfloat pointSize, normalColoring;
	Material* material;

public:
	Geometry(glm::mat4 currC, std::string objFilename, GLfloat pointSize, GLfloat normalColoring, Material* material);
	~Geometry();
	
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);

	void updatePointSize(GLfloat size);
	void updateNormalColoring(GLfloat normalColoring);
	void spin(float rotAngle, glm::vec3 rotAxis);
	void zoom(glm::vec3 s);
};

#endif