#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Node.h"

class Sphere : public Node {
	public:
		glm::mat4 modelView;
		glm::vec3 eyePos;

		GLuint vao = 0, vbo = 0, vbo_n = 0, ebo = 0;

		int stackCount = 400;
		int sectorCount = 400;
		int numsToDraw;

		Sphere(glm::mat4 currC, glm::vec3 eyePos);
		~Sphere();

		void draw(GLuint shaderProgram, glm::mat4 C);
		void update(glm::mat4 C);
		void spin(float deg);
};

#endif