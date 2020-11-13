#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"
#include "Material.h"
#include "LightSource.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> indices;

	GLuint VAO, VBO, NBO, EBO;
	
	GLfloat pointSize, normalColoring;
	Material* material;

public:
	PointCloud(std::string objFilename, GLfloat pointSize, GLfloat normalColoring, Material* material);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void updateNormalColoring(GLfloat normalColoring);
	void spin(float rotAngle, glm::vec3 rotAxis);
	void zoom(glm::vec3 s);
};

#endif