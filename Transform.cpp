#include "Transform.h"

Transform::Transform(glm::mat4 M)
{
	Transform::M = M;
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C)
{
	std::list<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->draw(shaderProgram, C * M);
	}
}

void Transform::update(glm::mat4 C)
{
	M = C * M;
}