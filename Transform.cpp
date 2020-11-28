#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1.0);
	mT = glm::mat4(1.0);
	mR = glm::mat4(1.0);
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C)
{
	std::list<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->draw(shaderProgram, projection, view, M * C * mT * mR);
	}
}

void Transform::update()
{
	rotate(0.0002f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::translate(glm::vec3 t)
{
	mT = glm::translate(glm::mat4(1.0), t) * mT;
}

void Transform::rotate(float rotAngle, glm::vec3 rotAxis)
{
	mR = glm::rotate(glm::mat4(1.0), glm::degrees(rotAngle), rotAxis) * mR;
}