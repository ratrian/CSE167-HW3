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

using namespace std;

class Geometry : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> indices;

	GLuint VAO, VBO, NBO, EBO;
	
	GLfloat pointSize, normalColoring;
	Material* material;

public:
	Geometry(std::string objFilename, GLfloat pointSize, GLfloat normalColoring, Material* material);
	~Geometry();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
	void update();

	void updatePointSize(GLfloat size);
	void updateNormalColoring(GLfloat normalColoring);
	void spin(float rotAngle, glm::vec3 rotAxis);
	void zoom(glm::vec3 s);
};

#endif