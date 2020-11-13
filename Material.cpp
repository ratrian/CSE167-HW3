#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	Material::ambient = ambient;
	Material::diffuse = diffuse;
	Material::specular = specular;
	Material::shininess = shininess;
}

void Material::sendMatToShader(GLuint shader)
{
	glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);
	glUniform1f(glGetUniformLocation(shader, "drawSphere"), 0.0);
}