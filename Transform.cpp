#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1);
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C)
{
	std::list<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->draw(shaderProgram, projection, view, M * C);
	}
}

void Transform::update(glm::mat4 C)
{

}

void Transform::translate(glm::vec3 t)
{
	glm::mat4 mT = glm::translate(glm::mat4(1.0), t);
	M = M * mT;
}

void Transform::rotate(float rotAngle, glm::vec3 rotAxis)
{
	// Update the model matrix by multiplying a rotation matrix
	glm::mat4 mR = glm::rotate(glm::mat4(1.0), glm::degrees(rotAngle), rotAxis);
	M = M * mR;
}