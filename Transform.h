#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <list>

class Transform : public Node
{
private:
	glm::mat4 M;
	std::list<Node*> children;

public:
	Transform();

	void addChild(Node* child);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
	void translate(glm::vec3 t);
	void spin(float rotAngle, glm::vec3 rotAxis);
};

#endif