#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

class Transform : public Node
{
private:


public:
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
};

#endif