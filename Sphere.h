#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere : public Object {
	public:
		GLuint vao = 0, vbo = 0, vbo_n = 0, ebo = 0, cubemapTexture;

		int stackCount = 40;
		int sectorCount = 40;
		int numsToDraw;
		Sphere();
		
		void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
		void update();
		void spin(float deg);
};

#endif