#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten)
{
	PointLight::pos = pos;
	PointLight::color = color;
	PointLight::atten = atten;
}

void PointLight::sendLightToShader(GLuint shader)
{
	glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shader, "lightCol"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shader, "lightAtten"), 1, glm::value_ptr(atten));
}

glm::vec3 PointLight::getPos() {
	return pos;
}

void PointLight::orbit(glm::vec3 direction, float rotAngle, glm::vec3 rotAxis)
{
	glm::mat4 mT = glm::translate(glm::mat4(1.0), direction);
	glm::mat4 mR = glm::rotate(glm::mat4(1.0), rotAngle, rotAxis);
	pos = mT * glm::vec4(pos, 1.0);
	pos = mR * glm::vec4(pos, 1.0);
}

void PointLight::move(glm::vec3 t)
{
	glm::mat4 mT = glm::translate(glm::mat4(1.0), t);
	pos = mT * glm::vec4(pos, 1.0);
}