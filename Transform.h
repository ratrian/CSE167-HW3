#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <list>

class Transform : public Node
{
private:
	glm::mat4 M, mT, mR;
	std::list<Node*> children;

public:
	Transform();

	void addChild(Node* child);
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C);
	void update();
	void translate(glm::vec3 t);
	void rotate(float rotAngle, glm::vec3 rotAxis);
};

#endif