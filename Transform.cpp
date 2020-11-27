#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1);
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C)
{
	std::list<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->draw(shaderProgram, M * C);
	}
}

void Transform::update(glm::mat4 C)
{
	M = M * C;
}